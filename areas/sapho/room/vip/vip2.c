// vi2.c - VIP Lounge Entryway - Bebop 2021

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$WHITEThe $YELLOWVIP$OFF Lounge"));
    set_long(
"You have entered what appears to be the VIP Lounge for high-ranking members\n"
+"of this sapho operation. This place is decked out lavishly with beautiful\n"
+"red plush carpet, velvet furniture and ornate tapestries from some ancient\n"
+"culture or civilization. While the decor sure is nice, the people in this\n"
+"room have a certain...psychotic look in their eyes. The overall vibe seems\n"
+"to be that of a party, albeit a very intense one. There's a large marble\n"
+"countertop along the edge of the room, with people crowded all around it,\n"
+"seemingly raving over whatever is on the surface. People around you are\n"
+"yelling and shouting, drinking heavily, and seemingly not caring the least\n"
+"about your presence. With the exception of the large bouncers, of course.\n");

    add_exit("door", ROOM_DIR + "fhall4.c");
    add_exit("west", ROOM_DIR + VIP + "vip1.c");
    add_exit("east", ROOM_DIR + VIP + "vip3.c");

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

  place_objects(NPC_DIR + "bouncer.c", 3);

}

void init(){
  ::init();

  add_action("snort","snort");
  add_action("move", "west");
  add_action("move", "east");
  add_action("kill", "kill");

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

// NOTE: bounce_player(str type) is defined
// in /include/area_utils.c !!!

// If the player tries to move past the bouncer, they 
// get blocked. If they don't have a keycard, they
// get bounced.

int move()
{
  return bounce_player("move");
}

// If the player has no keycard and tries to kill any bouncers,
// they get thrown out of the area.

int kill (string str){
  object keycard;
  if (str != "bouncer") return notify_fail("Kill what ?");
  return bounce_player("kill");
}

