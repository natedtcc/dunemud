# DuneMUD

This repo contains a small bit of code I've done / am currently working on for
DuneMUD. This is a game that was first brought online back in 1992, and has maintained
a dedicated playerbase for almost 30 years now. The game is played over Telnet (like other
MUDs/MUSHes/MOOs etc), and the codebase is written in LPC, a specialized flavor of C written
specifically for MUDs.

Play now!

telnet://dunemud.net:6789

or from CLI:

telnet dunemud.net 6789

More on LPC:

https://www.cs.hmc.edu/~jhsu/wilderness/basics.html


New Quest Additions


sdesc: "The Lost Crysknife"

ldesc:

A powerful Fremen tribe is in an uproar. Their sietch was recently
the victim of a brutal Harkonnen military campaign, which resulted
in the theft of a sacred crysknife. A Wise Fremen seeks an audience
with anyone who is able to procure this plundered treasure; Seek him
out! He can be found in a Fremen Sietch, in the northern part of the
Arrakis Desert.

Quest points: 5 (it's tough, especially for Dune newbs)

Quest ID: "stolen_knife"

/d/Fremen/bebop/sietch/quest_obj.c

--------------------------------------------------------------------

sdesc: "Tainted Spice"

ldesc:

There's a large, angry chef at the Tleilax Hospital who's been
getting bad batches of spice lately. You hear that the residents
of the Hospital have been getting even sicker, and the tainted
spice is likely to blame. The chef will reward anyone who can
provide him with a clean product. Can you find the pure, liquid
spice stashed away within the Hospital grounds? 

Quest points: 3

Quest ID: "tainted_spice"
