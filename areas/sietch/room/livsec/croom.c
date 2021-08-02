#define CROOM "/d/Fremen/domain/guild/croom.c"
inherit "/room/newroom";
#include "/d/Fremen/bebop/sietch/include/defines.h"
#include "/d/Fremen/guild/include/defines.h"
#include "ansi.h"

status check_equip();

int challenge;

void reset(int arg)
{
	if (arg)
		return;
	set_light(1);
	short_desc = "Circle of Equals";
	long_desc = "\
The circle of equals is a rough circle of sand with the borders\n\
defined by stones. The circle was built for Fremen who desired\n\
to gain a higher rank. There can be no escape from the circle and\n\
the fight is till one or the other falls. Enter fully healed and fight\n\
with everything you've got.\n" +
							RED + "The Fremen Bank is back in business!" + OFF + "\n";
	items = (["circle":"The circle is made of sand and defined by rocks.",
						"stones":"The stones are set in the shape of a large circle.",
							"sand":"Sand is what makes up the circle.",
	]);
	dest_dir = (["east":ROOM_DIR + LIVSEC + "croom5.c",
							 "west":ROOM_DIR + LIVSEC + "croom4.c",
							"south":ROOM_DIR + LIVSEC + "croom7.c",
							"north":ROOM_DIR + LIVSEC + "croom2.c",
	]);
	verbs = (["summon":"do_summon",
							"step":"do_step",
	]);
}
/*
	status do_summon(string str){
	if(!str || str != "elders"){
		notify_fail("Summon who?\n");
		return 0;
	}
	if(!FSOUL) {
	  write("you are missing your fremen suit, replace it at the guild.\n");
	  return 1;
	}
	if((int)FSOUL->query("challenge") != 1){
		write("You are not ready to challenge for a higher rank.\n");
		return 1;
	}
	write("You summon a Fremen your equal and the elders to witness the\n\
battle. Fight when you are totally healed and fight with every\n\
bit of strength you can muster for there can be no escape.\n\
When you are ready, 'step into circle'.\n");
	challenge = 1;
	return 1;
}
	status do_step(string str){
	if(!str)
		return 0;
	if(str != "into circle"){
		notify_fail("Step where?\n");
		return 0;
	}
	if(!challenge){
		write("You have to summon the elders first to witness the battle.\n");
		return 1;
	}
	check_equip();
	if(check_equip()){
		write("Not allowed to enter.\n");
		return 1;
	}
	write("You step into the circle.\n");
	call_other(CROOM, "???");
	move_object(this_player(), find_object(CROOM));
	return 1;
}
	status check_equip(){
	object *inv;
	int i, j;
	inv = all_inventory(this_player());
	j = sizeof(inv);
	for(i=0;i<j;i++){
		if(inv[i]->query_armour())
			if(inv[i]->drop() != 0)
				return 0;
			else {
				tell_object(this_player(),
				  "You cannot bring your "+inv[i]->query_name()+" in.\n");
				return 1;
			}
		if(inv[i]->query_weapon() && !inv[i]->query_fremen_item()){
			tell_object(this_player(),
			  "You cannot bring your "+inv[i]->query_name()+" in.\n");
			return 1;
		}
	}
}
 */
