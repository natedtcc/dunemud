inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";
reset(arg) {
  if(!present("minister"))
    move_object(clone_object("/d/Atreid/alan/hall/npc/minister"), this_object());
  if(!present("councilor") && random(5) > 2)
    move_object(clone_object("/d/Atreid/alan/hall/npc/councilor"), this_object());
  if(arg)return;
  set_light(1);
  short_desc = "Research room";
  long_desc = "\
You enter a mid-sized room, where all of the walls are covered\n\
with research equipment of some kind. You see files full of\n\
ridulan crystal, bookcases stacked high with books, and even some\n\
electronic info-searchers. This room contains all of the city's laws\n\
as well as those of the entire feif of Caladan. Ministers often come\n\
here, either to research or meet with an individual town councilor.\n";
  items = ([
    "room":"It is used for research and small meetings",
    "walls":"They are filled with information and research equipment.",
    "equipment":"Ridulan crystal, books, info-seekers ... the works, including a strange\n"+
    "looking power socket of Ixian manufacture.",
    "socket":"It looks like a power port, it is marked with the letters 'I.D.'.",
    "files":"Typical file cabinets, filled with milimeter thick ridulan crystal.",
    "books":"Classicaly bound books, mostly containing the very ancient laws.",
    "seekers":"They electronically scan all data available to them",
    "info-seekers":"=seekers",
    "laws":"They are contained in the volumnes and files here.",
    ]);
  dest_dir = ([
    "west":"/d/Atreid/alan/hall/hall1",
   ]);
}

//Function added for IMG recharge - Prax Aug/19/00
// int power_drain() { return 15000; }
