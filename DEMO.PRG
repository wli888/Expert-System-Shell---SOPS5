(literalize lesuur nummer)
(literalize dag naam)
(literalize vak naam)
(literalize groep nummer)
(literalize rooster dag lesuur gepland vak groep)
(literalize hulp_tijdschema dag lesuur bezet vak groep)
(literalize start_planning)

(p init_rooster
; initialiseer rooster (per groep) met vakken die nog gepland moeten worden
	(vak ^naam <vaknaam>)
	(groep ^nummer <groepnummer>)
	- (rooster ^vak <vaknaam> ^groep <groepnummer>)
-->
	(make rooster ^gepland nee ^vak <vaknaam> ^groep <groepnummer>))

(p init_hulp_tijdschema
; initialiseer een hulp tijdschema (per groep).
; het hulp tijdschema dient om bij te houden
; - of op een bepaald tijdstip een vak is gepland,
; - of een poging gedaan is om een vak op die tijdstip te plannen, maar
;   faalde omdat er niet aan de restricties voldaan is
;   (restricties voor plannen zie hieronder),
; - of op een bepaald tijdstip nog geen vak gepland is
	(lesuur ^nummer <lesuurnummer>)
	(dag ^naam <dagnaam>)
	(groep ^nummer <groepnummer>)
	- (hulp_tijdschema ^dag <dagnaam> ^lesuur <lesuurnummer> ^groep <groepnummer>)
-->
	(make hulp_tijdschema ^dag <dagnaam> ^lesuur <lesuurnummer> ^bezet nee ^groep <groepnummer>))

(p maak_start_planning
; zodra een rooster en een hulp tijdschema geinitialiseerd is kan een
; aanduiding gemaakt worden dat het plannen van het rooster kan beginnen
        (rooster ^vak nederlands ^groep 1 ^gepland nee)
        (hulp_tijdschema ^dag maandag ^lesuur 1 ^bezet nee ^groep 1)
	- (start_planning)
-->
        (make start_planning))

(p maak_rooster
; een vak (voor een groep) kan op een bepaald tijdstip gepland worden als
; er aan de volgende restricties wordt voldaan:
; - als er op het tijdstip niet hetzelfde vak gepland is voor een andere groep
; - als er op het tijdstip niet een ander vak gepland is voor dezelfde groep
        (start_planning)
	{ <hulp_tijdschema> (hulp_tijdschema ^dag <dagnaam> ^lesuur <lesuurnummer> ^groep <groepnummer>) }
	{ <rooster> (rooster ^groep <groepnummer> ^vak <vaknaam> ^gepland nee) }
	- (rooster ^groep <> <groepnummer> ^vak <vaknaam> ^gepland ja ^dag <dagnaam> ^lesuur <lesuurnummer>)
	- (rooster ^groep <groepnummer> ^vak <> <vaknaam> ^gepland ja ^dag <dagnaam> ^lesuur <lesuurnummer>)
-->
	(modify <rooster> ^gepland ja ^dag <dagnaam> ^lesuur <lesuurnummer>)
	(modify <hulp_tijdschema> ^bezet ja ^vak <vaknaam>))

(p markeer_geweest
; wanneer er een poging wordt gedaan om een vak op een bepaald tijdstip te
; plannen, maar faalt omdat er niet aan bovenstaande restricties voldaan kan
; worden, dan wordt het tijdstip gemarkeerd met "geweest"
        (start_planning)
	{ <hulp_tijdschema> (hulp_tijdschema ^dag <dagnaam> ^lesuur <lesuurnummer> ^groep <groepnummer>) }
	(rooster ^groep <groepnummer> ^vak <vaknaam> ^gepland nee)
	(rooster ^groep <> <groepnummer> ^vak <vaknaam> ^gepland ja ^dag <dagnaam> ^lesuur <lesuurnummer>)
	- (rooster ^groep <groepnummer> ^vak <> <vaknaam> ^gepland ja ^dag <dagnaam> ^lesuur <lesuurnummer>)
	- (hulp_tijdschema ^dag <dagnaam> ^lesuur <lesuurnummer> ^bezet geweest ^groep <groepnummer>)
-->
	(modify <hulp_tijdschema> ^bezet geweest ^vak <vaknaam>))

(p verwissel
; wanneer er nog tijdstippen vrij zijn om een vak te plannen, maar er kan
; geen planning gemaakt worden vanwege de restricties, dan wordt deze vak
; verwisseld met een al gepland vak van dezelfde groep, zodat het vak toch
; gepland kan worden
        (start_planning)
	{ <rooster1> (rooster ^groep <g1> ^vak <v1> ^gepland nee) }
	- (hulp_tijdschema ^bezet nee ^groep <g1>)
	{ <hulp_tijdschema1> (hulp_tijdschema ^dag <dagnaam1> ^lesuur <lesuurnummer1> ^bezet geweest ^vak <v1> ^groep <g1>) }
	{ <hulp_tijdschema2> (hulp_tijdschema ^dag <dagnaam2> ^lesuur <lesuurnummer2> ^bezet ja ^vak { <v2> <> <v1> } ^groep <g1>) }
	{ <rooster2> (rooster ^dag <dagnaam2> ^lesuur <lesuurnummer2> ^vak <v2> ^groep <g1> ^gepland ja) }
	- (rooster ^dag <dagnaam2> ^lesuur <lesuurnummer2> ^vak <v1> ^groep <> <g1> ^gepland ja)
-->
	(modify <hulp_tijdschema1> ^dag <dagnaam2> ^lesuur <lesuurnummer2> ^bezet ja)
	(modify <hulp_tijdschema2> ^dag <dagnaam1> ^lesuur <lesuurnummer1>)
	(modify <rooster1> ^gepland ja ^dag <dagnaam2> ^lesuur <lesuurnummer2>)
	(modify <rooster2> ^dag <dagnaam1> ^lesuur <lesuurnummer1>))

(p print_rooster
; druk het rooster af op het scherm, wanneer alle vakken gepland zijn
	(rooster ^dag <dagnaam> ^lesuur <lesuurnummer> ^gepland ja ^groep <groepnummer> ^vak <vaknaam>)
	- (rooster ^gepland nee)
-->
	(write (crlf))
	(write rooster <dagnaam> lesuur <lesuurnummer> groep <groepnummer> vak <vaknaam>)) 
