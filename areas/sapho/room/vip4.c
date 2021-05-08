// vip4.c - VIP Lounge Entryway - Bebop 2021

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  if (arg)
    return;

  set_light(1);
  set_short("$WHITEThe $YELLOWVIP$OFF Lounge");
  set_long(
      "You have entered what appears to be the VIP Lounge for high-ranking members of this sapho\n"
      +"operation. This place is decked out lavishly with beautiful red plush carpet, velvet furniture\n"
      +"and ornate tapestries from some ancient culture or civilization. While the decor sure is nice,\n"
      +"the people in this room have a certain...psychotic look in their eyes. The overall vibe here\n"
      +"seems to be that of a party, it seems to be a very intense one. There's a large marble countertop\n"
      +"along the edge of the room, with people crowded all around it, seemingly raving over whatever\n"
      +"is on the surface. People around you are yelling and shouting, drinking heavily, and seemingly\n"
      +"not caring the least about your presence, with the exception of the large bouncers, of course.");

  add_exit("door", ROOM_DIR + "fhall4.c");
  add_exit("north", ROOM_DIR + "saferoom.c");

  add_item("bouncers",
      "Big muscular dudes with almost no visible neck.");

  add_item("carpet",
      "Quite soft, and very clean given the circumstances.");

  add_item("furniture",
      "A few large sofas, loveseats and recliners adorn this area.");

  add_item("people",
      "The people here are definitely dressed up to party, but man do they look intense.");
  
  add_item("countertop",
      "As you get closer to the counter, you realize these people are going crazy over some powder \n"
      +"with a red tint to it on a mirror.");

  add_item("counter",
      "As you get closer to the counter, you realize these people are going crazy over some powder \n"
      +"with a red tint to it on a mirror.");
  
  add_item("powder",
      "Upon closer expection, yup, you've confirmed it, these crazy bastards made snortable sapho.\n"
      "No wonder these people are so geeted.");
}

  void init(){
::init();

  add_action("snort","snort");
}
  // Make bad life choices

  int snort(string str){
    if (str != "powder") {return notify_fail("What ?\n");}
    else {
      write("**SNIFFF**\nUPON EVEN CLOSER INSPECTIOONAAARRRGGUHHH!!!\n"
        +"You flex your muscles and scream: HELL YEAH!!\nYou slam your fist "
        +"on the counter, shattering the mirror.\n");
      return 1;
    }
  }