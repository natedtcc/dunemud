
/* Caladan Astroport
*
*  Modified by Hayt, 9-14-05 for extra yumminess!
*      -  Added all the stuff from the old astroport.
*      -  Fixed a few stryder errors.
*      -  Put into the game.
*
*  Modified by Kbl to put new space system
*  4/23/2008
*  This is some horrible shit right here. Added a bit better interface for ordering ships. -Rav
*  Added the new trivia_room to cal_station and commented out the
*  bit blocking players from entering. - Bebop 08/03/2021

*/

inherit "room/newroom";

#include "ansi.h"

#define TP this_player()
#define QL query_level()
#define CALMAP "/d/Ix/kbl/Base/obj/map1"
#define TOPLIST "/obj/etc/toplist"
#define SCORELIST "/obj/etc/scorelist"
#define SPACESCHEDULE "/d/Space/ravien/ssched"

void reset(int arg)
{
    ::reset(arg);
    if(!present("schedule"))
      move_object(clone_object(SPACESCHEDULE),this_object());
    if(arg)return;

    set_light(1);
    place_objects("/obj/atm",1);
    place_objects(CALMAP,1);
    place_objects(TOPLIST,1);
    place_objects(SCORELIST,1);

    set_short("Caladan Astro Port");
    set_long("\
Caladan Spacing Guild Astroport. You are in the center of a ring of\n\
buildings. With all the man made lighting in this area the moon looks\n\
like a dim faded disk in the sky, and no stars can be seen. Major roads\n\
lead in all directions from here, and you can see many buildings lining\n\
them. Even this late at night people are everywhere, rushing to and fro,\n\
in a desperate attempt to reach their destination. A large illuminated\n\
sign above the northern road says: 'Welcome to Caladonia - Metropolis by\n\
the Sea'. An important looking display is right next to you.\n\
"+BOLD+"Newbies: Use portal to access small areas."+OFF+"\n");

    long_night_desc = "\
Caladan Spacing Guild Astroport. You are in the center of a ring of\n\
buildings. With all the man made lighting in this area the moon looks\n\
like a dim faded disk in the sky, and no stars can be seen. Major roads\n\
lead in all directions from here, and you can see many buildings lining\n\
them. Even this late at night people are everywhere, rushing to and from,\n\
in a desperate attempt to reach their destination. A large illuminated\n\
sign above the northern road says: 'Welcome to Caladonia - Metropolis by\n\
the Sea'. An important looking display is right next to you.\n\
Newbies: Use portal to access small areas.\n";

    dest_dir = ([
      "west":     "/d/Atreid/domain/main1w",
      "north":    "/d/Atreid/domain/north1",
      "portal":   "/d/Atreid/domain/island",
      "south":    "/d/Atreid/domain/south1",
      "east":     "/d/Atreid/domain/main1e",
      "up":       "/d/Atreid/domain/trivia/trivia_room",
      "down":     "/d/Atreid/domain/concern_room",
      "academy":  "/d/Newbie/school/room/room1",
    ]);
    items = ([
      "sign":"A large sign, reading 'Welcome to Caladonia - Metropolis by the Sea'.",
      "spacing guild astroport":
      "You're standing at the edge of the landing pad surrounded by several low buildings.",
      "guild astroport": "=spacing guild astroport",
      "astroport": "=spacing guild astroport",
      "landing pad": "Place where guild ships will land.",
      "pad": "=landing pad",
      "buildings": "Several surrounding the landing pad.",
      "display":"For help with Spacing Guild ship commands type: help space. Have a nice day.",
      "display terminal":
      "It's a weather resistant display with a label on it.",
      "terminal": "=display terminal",
      "label": "It has these words on it: help space.",
      "pedestrians":"A mingling of Caladonian residents and travelers from abroad.",
    ]);
    set_planet("caladan");
    set_property("no_hide");
    set_property("no_voice");
    set_property("no_steal");
    set_property("no_rndmove");
    set_outdoor();
}

int portal()
{
    if(TP->QL > 5 && !wizardp(TP))
    {
        write("Your body is too big to enter the portal!\n");
        return 1;
    }
    write("You step into the portal and see a flash of blue light.\n\
When you can see again you realize you are traveling at great\n\
speeds. You finally arrive at a similar portal ... you look around:\n");
    TP->move_player("through the flaming portal","/d/Atreid/domain/island");
    return 1;
}

// int up()
// {
//     object uproom;
//     uproom = find_object(dest_dir["up"]);
//     if(!wizardp(TP))
//     {
//         if(!uproom)
//           {
//             write("No Trivia is being played!\n");
//             return 1;
//           }
//         if(!present("pursuit", uproom))
//           {
//             write("No Trivia is being played!\n");
//             return 1;
//           }
//     }
//     TP->move_player("up","/d/Atreid/domain/TriviaRoom");
//     return 1;
// }

void init()
{
    ::init();
    add_action("portal","portal");
    // add_action("up","up");
}

string query_port() { return "caladan"; }
