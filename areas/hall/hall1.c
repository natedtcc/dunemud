inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";
reset(arg) {
  if(!present("councilor"))
    move_object(clone_object("/d/Atreid/alan/hall/npc/councilor"), this_object());
  if(arg)return;
  set_light(1);
  short_desc = "Middle of second story";
  long_desc = "\
This is the middle room of the second story of Caladonia's\n\
city hall. To the west, you see the stairwell, while to the\n\
east is the research room, where minister's of different parts\n\
of the city government come to meet with city councilors and find\n\
data. North is the main conference room, where the city councilors\n\
meet to make policy. Southerly is the bookeeping office, where the\n\
city secretaries and other employee's work.\n";
  items = ([
    "room":"The middle room of the second floor. Used by high ranking officials.",
    "stairwell":"It can take you up or down.",
    "research room":"Ministers from their respective offices come to city hall to meet with\n\
the councilors and gather data.",
    "research":"=research room",
    "office":"Secretaries and functionaries work there, keeping books.",
   ]);
  dest_dir = ([
    "west":"/d/Atreid/alan/hall/midstair",
    "east":"/d/Atreid/alan/hall/research",
    "north":"/d/Atreid/alan/hall/conference",
    "south":"/d/Atreid/alan/hall/book",
   ]);
}

