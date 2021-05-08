/// sapho5.c - Sapho Den - Bebop 2019

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  place_objects(NPC_DIR + "junkie.c", 2);

  // Spawn the PusherMan

  if (!present("pusherman"))
  {
    move_object(clone_object(NPC_DIR + "pusherman.c"), this_object());
  }

  if (arg)
    return;

  set_light(1);
  set_short("$BOLD$WHITEA $REDSapho $WHITEDen$OFF");
  set_long(
      "You find youself in the center of the den, with junkies all about. A\n" + "couple of them appear to be talking to a shady-looking fellow in the\n" + "corner of the room. The smell here is quite powerful.\n");

  add_exit("east", ROOM_DIR + "sapho6.c");
  add_exit("south", ROOM_DIR + "sapho2.c");
  add_exit("north", ROOM_DIR + "sapho8.c");
  add_exit("west", ROOM_DIR + "sapho4.c");

  add_item(
      "pipes", "Curved, elongated pipes, all full with red-tar sapho.");

  add_item(
      "junkies", "It looks like they're buying Red Tar Sapho from the Pusher Man..");
}

init()
{
  ::init();

  /// Add actions for buying sapho / pipe

  add_action("do_buy", "buy");
  add_action("do_list", "list");
  add_action("do_sell", "sell");
}

int do_buy(string str)
{
  /// Check to make sure the drug dealer is available...

  if (present("pusherman"))
  {

    /// If the player wants sapho...

    if (str == "sapho")
    {

      /// Check the player has enough cash...

      if (this_player()->query_money() < 5000)
      {
        return notify_fail("Yo, this ain't no charity. That cost 5000 solaris.\n");
      }

      /// Dispense drugs.

      place_objects(OBJ_DIR + "sapho.c", 1, this_player());
      this_player()->add_money(-5000);
      tell_object(
          TP, "Pusher Man says: Here's your sapho, kid. I'm sure I'll see ya" + " around...\nPusher Man gives you a rock of $BOLD$REDSapho$OFF.\n");

      return 1;
    }

    /// If the player wants a pipe...

    if (str == "pipe")
    {

      // Same as above...

      if (this_player()->query_money() < 1000)
      {
        return notify_fail(
            "Ya god damn bum! That cost 1000 solaris!\n");
      }

      place_objects(OBJ_DIR + "pipe.c", 1, this_player());
      this_player()->add_money(-1000);
      tell_object(
          TP, "Pusher Man says: Ok, here's a pipe. Want me to smoke it for you" + " too??\nPusher Man gives you a small sapho pipe.\n");

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


int do_sell(string str){

  if (str != "sapho" || !present("redtar_loot", this_player())) {
    return notify_fail(
      "Pusher Man says: Sorry bub, I only buy the massive chunks of sapho!\n")
  }

   this_player()->add_money(50000);
  
  return 1;


}