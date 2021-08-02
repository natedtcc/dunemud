// sapho5.c - Sapho Den - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$BOLD$WHITEA $REDSapho $WHITEDen$OFF"));
    set_long(
      "You find youself in the center of the den, with junkies all about. A\n"
      +"couple of them appear to be talking to a shady-looking fellow in the\n"
      +"corner of the room. The smell here is quite powerful.\n");

    add_exit("east", ROOM_DIR + DEN + "den6.c");
    add_exit("south", ROOM_DIR + DEN + "den2.c");
    add_exit("north", ROOM_DIR + DEN + "den8.c");
    add_exit("west", ROOM_DIR + DEN + "den4.c");

    add_item(
      "pipes", 
        "Curved, elongated pipes, all full with red-tar sapho.");

    add_item(
      "junkies", 
        "It looks like they're buying Red Tar Sapho from the Pusher Man..");
  }

  place_objects(NPC_DIR + "junkie.c", 2);
  place_objects(NPC_DIR + "pusherman.c", 1);
  
}

init()
{
  ::init();

  // Add actions for buying sapho / pipe

  add_action("do_buy", "buy");
  add_action("do_list", "list");
  add_action("do_sell", "sell");
}

int do_buy(string str)
{
  // Check to make sure the drug dealer is available...

  if (present("pusherman"))
  {

    // If the player wants sapho...

    if (str == "sapho")
    {

      // Check the player has enough cash...

      if (this_player()->query_money() < 5000)
      {
        return notify_fail("Yo, this ain't no charity. That cost 5000 solaris.\n");
      }

      // Dispense drugs.
      object sapho = clone_object(OBJ_DIR + "sapho.c");
      move_object(sapho, this_player());
      this_player()->add_money(-5000);
      tell_object(
          this_player(), "Pusher Man says: Here's your sapho, kid. I'm sure I'll see ya"
          + " around...\nPusher Man gives you a rock of $BOLD$REDSapho$OFF.\n");

      return 1;
    }

    // If the player wants a pipe...

    if (str == "pipe")
    {

      // Same as above...

      if (this_player()->query_money() < 1000)
      {
        return notify_fail(
            "Ya god damn bum! That cost 1000 solaris!\n");
      }

      // Dispense paraphernalia
      object pipe = clone_object(OBJ_DIR + "pipe.c");
      move_object(pipe, this_player());
      this_player()->add_money(-1000);
      tell_object(
          this_player(), "Pusher Man says: Ok, here's a pipe. Want me to smoke it for you"
          +" too??\nPusher Man gives you a small sapho pipe.\n");

      return 1;
    }

    if (str != "pipe" || str != "sapho")
      return notify_fail("I only sell pipes and sapho!\n");
  }

  else
    return notify_fail(
        "You'll have to wait for the Pusher Man to come back.\n");
}

int do_list(string str)
{
  write(
      "Yo, I got you. Just type 'buy sapho' for some red tar, " + "or 'buy pipe'.\n"
      +"I also buy large masses of red tar for 50,000 solaris a pop.\n");
}

// Sell sapho to the Pusher Man
// sapho_loot is obtained by players in /room/packaging/pack1, pack10 & pack12

int do_sell(string str){

  object sapho_loot;

  if (!present("pusherman")) {
    return notify_fail(
      "You'll have to wait for the Pusher Man to come back.\n");
    }
  if (str != "sapho" || !(sapho_loot = present("sapho_loot", this_player()))) {
    return notify_fail(
      "Pusher Man says: Sorry bub, I only buy the large masses of sapho!\n");
  }
    tell_object(
      this_player(), 
      "Pusher Man says: That's a nice mass of red tar!\n"
      +"The Pusher Man snatches your large mass of sapho and gives you " 
      + SAPHO_LOOT_VALUE + " solaris.\n");
   this_player()->add_money(SAPHO_LOOT_VALUE);
   destruct(sapho_loot);
  
  return 1;

}
