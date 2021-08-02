// deathpit.c - Trap of doom - Bebop 2021

// Player will get stuck here and have to:
// 'exa skeleton', 'exa bones', 
// 'insert bones into holes', 'climb bones'

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

// Include keycard generator
inherit "/d/Imperial/bebop/sapho/include/keycard_gen.c";

// Bool for checking if the 'bones' have been 'insert'ed
int has_inserted;

void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short("An empty pit");
    set_long(
"You've tumbled into a deep, dark pit, with no discernable exits around you. The\n"
+"walls here are made from stone, smoothly finished to negate any chance of you\n"
+"climbing them. There's a skeleton sitting in the corner, which helps spread the\n"
+"feelings of fear throughout your entire body. You wonder how the hell you're\n"
+"going to get out of here...");

    add_item("skeleton",
      "This thing is definitely aged. For some reason, the bones are sharp to a point.");
    add_item("bones",
      "Long and sharply pointed femur bones.\nYou might be able to 'insert' them into something.");
    add_item("walls",
      "Smooth stone walls with no notches. However, there are some small holes present \n"
      +"on the survace.");
    add_item("wall",
      "Smooth stone walls with no notches. However, there are some small holes present \n"
      +"on the survace.");
    add_item("holes",
      "Somewhat deep and a few inches across.\nYou might be able to 'insert' something into them.");
  }

  has_inserted = 0;

}

  void init(){
::init();

  add_action("do_insert","insert");
  add_action("do_climb","climb");
}

// Prepare the holes for climbing

int do_insert(string str){
  if (str != "bones into holes"){return notify_fail("Insert what into what??\n");}

  else if (has_inserted) {return notify_fail("The bones are already inserted in the wall!\n");}

  write("You insert the bones into the wall. Maybe you could try to 'climb' them?\n");
  has_inserted = 1;
  return 1;
  
}

// Climb out of the pit

int do_climb(string str){
  
  if (!has_inserted) {
    return notify_fail("What ?\n");
  }
  
  else if (str != "bones"){
    return notify_fail("Insert what into what??\n");
  }
  // FREEDOM!
  write("\nYou climb the bones and manage to open the trapdoor you fell into, and escape!\n");
  this_player()->move_player(
    "disappears from sight", ROOM_DIR + "fhall4.c");
    
  // Cant climb no mo'
  has_inserted = 0;

  // Attempt to generate a new keycard
  LOGGER->writeLog(
    this_player()->query_name() + " attempting to spawn a keycard (deathpit) @ " + ctime(time()));
  keycard_gen();
  
  return 1;
  
}