#include <mudlib.h>
inherit ROOM;
do_nord();
int moved;
void reset(int arg) {
        ::reset(arg);
        if(arg)
                return;
  set_light(1);
  short_desc = "Wallach Ward Tunnel Entrance";
  long_desc = "\
This is the entrance to the Ward Tunnel. You hear a shout\n\
and a lasgun shot from the tunnel. To your west is the north\n\
Ward Hallway. Go north to enter the tunnel. If you are above\n\
level 10, you will skip over the tunnel when you go north.\n\
There is a table here, accompanied by three chairs. There is\n\
also a $REDsign$OFF here.\n";
  items = ([
    "entrance":"Wallach Ward Tunnel Entrance",
    "ward":"Wallach Ward Tunnel Entrance",
    "tunnel":"Go north to enter the tunnel.",
    "ceiling":"The ceiling here is completed.",
    "sign":"$BOLD$RED  \/---------------------------\\$OFF\n"+
           "$BOLD$RED \/$CYANGo north to enter the tunnel,$RED\\$OFF\n"+
           "$BOLD$RED\/ $CYANBut be sure to use 'consider' $RED\\$OFF\n"+
           "$BOLD$RED\\ $CYANBefore attacking random mobs. $RED\/$OFF\n"+
           "$BOLD$RED \\$CYANAlso, remember to read signs!$RED\/$OFF\n"+
           "$BOLD$RED  \\---------------------------\/$OFF\n",
    "table":"It's oak. It appears to be broken.",
    "chairs":"Try 'sitting' in one?",
  ]);

/* Instead of calling "'#do_nord" as the north
*  direction, use the actual direction...
*  Bebop 2021
*/

dest_dir = ([
    "west":"/d/Gesserit/tick/area1/12.c",
    "north":"/d/Gesserit/tick/area1/23.c"
  ]);
}

init() {
  ::init();
  add_action("sit","sit");
  add_action("search","search");
  add_action("do_nord","north");
}
sit(string str) {
  if(!str)
  return notify_fail("Sit on what?\n");
  if(str == "on chair") {
    write("The chair is too big to sit on.\n");
  }
  return 1;
}
search(string str) {
  write("This isn't a quest... stop searching.\n");
  return 1;
}

/*
* Cleaned up this function a bit - there was a bit of spaghetti
* going on. 
* -Bebop 2021
*/

do_nord()
{
  // Wiz check
  if (this_player()->query_wizard()) {
    this_player()->move_player("north","/d/Gesserit/tick/area1/23.c");
  }
  
  // Assess worthiness
  else if ((this_player()->query_guild() == "Fremen") 
    || (this_player()->query_guild() == "Atreid")) {
      
    int lvl = this_player()->query_level();
    
    // Newbie entrance
    if (lvl<10){
      write("\nYou move quickly into the tunnel\n");
      this_player()->move_player("north","/d/Gesserit/tick/area1/23.c");
    }

    // Bit further in the area for bigger players
    else if (lvl>9 && lvl<16){
      write("\nYou fly past the tunnel, into the ward\n");
      this_player()->move_player("north","/d/Gesserit/tick/area1/pt/52.c");
    }

    // Frig off
    else {
      write("Sorry, you're too big to enter here.\n");     
    }
  }

  // Sorry, not worthy. Good thing there's nothing in here.
  else {
    write("You don't seem to be worthy to enter. You need to be of Atreides or Fremen ancestry.\n");
  }

  return 1;
}
