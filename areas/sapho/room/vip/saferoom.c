// saferoom.c - Boss room / quest item location - Bebop 2018
#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short("$BOLD$WHITEGabriel's Saferoom$OFF");
    set_long(
"You've stumbled in to the saferoom of Gabriel, the murderous drug lord of \n"
+"Salusa. The walls are adorned with the heads of various rare creatures,\n"
+"as well as priceless artifacts undoubtedly plundered from somewhere on \n"
+"Arrakis. There's piles of solaris in every direction and you get the\n"
+"feeling Gabriel doesn't want you around. There's a large ornate desk\n"
+"in the center of the room.");

    add_exit("north", ROOM_DIR + VIP + "vip7.c");

    add_item(
      "solaris", "A metric fuckton of solaris.");
    
    add_item(
      "desk", "A large ornate desk, with one big drawer in the center.");

    add_item(
      "drawer", "A big drawer, containing who knows what. Maybe try searching it?");
    
  }

  place_objects(NPC_DIR + "gabriel.c", 1);

}

void init()
{
::init();

  add_action("search", "search");
}

// Search the drawer to find a recipe quest item

int search(string str){

  object recipe;

  if (str != "drawer")
    return notify_fail("Search what?\n");
  
  else if (present("gabriel"))
    return notify_fail("Gabriel slaps your hand away from his desk drawer.\n");

  else if ((recipe = present("recipe_quest_item", this_player())))
    return notify_fail("You already have the recipe!\n");

  recipe = clone_object(OBJ_DIR + "recipe_quest.c");
  
  move_object(recipe, this_player());

  write(
    "You rifle around in the desk and find the red tar sapho recipe!\n"
    +"You quickly take it.\n");
  
  return 1;

}