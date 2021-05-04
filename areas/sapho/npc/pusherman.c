#include <mudlib.h>
inherit "/obj/player/monster";

void reset(int arg)
{
  ::reset(arg);
  if(arg) return;

  set_name("pusherman");
  set_alias( ({"pusherman", "dealer", "pusher"}) );


  set_short("The $BOLDPusher$OFF Man");
  set_long(
     "He's a drug dealer. This guy is the one supplying the whole entire\n"
     +"den with $BOLD$REDSapho$OFF. You wonder if you can 'buy' anything"
     +" from him.....\n'God damn! The Pusher Man..'\n");

  set_gender(1);
  set_race("fremen");

  set_level(50);
  set_non_combat_chats (10, ({
    ({ "Pusher Man yells: 'SQUAALAYYYY!' and makes various hand gestures.", "echo" }),
	({ "Pusher Man takes a drag of his cig, spits, and says: It's fuckin' nice out!" }),
	({ "Pusher Man says: I sell pipes to go with your sapho. I got everything, man!" }),
    ({ "Pusher Man says: I got dat Red Tar Sapho if you lookin'... ", "echo"}),
    ({ "Pusher Man yells: Yo, hit my line! Either DM me or hit my line! I got that loud.","echo" }) }) );
  
}

