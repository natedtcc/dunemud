// area_utils.c -  Sapho Den / Refinery Utils / Consts - Bebop 2020

// Define directory location strings

string ROOM_DIR = "/d/Imperial/bebop/sapho/room/";
string NPC_DIR = "/d/Imperial/bebop/sapho/npc/";
string OBJ_DIR = "/d/Imperial/bebop/sapho/obj/";
string DEN = "den/";
string PROC = "processing/";
string PACK = "packaging/";
string VIP = "vip/";
string EQUIP = "equip/";
object LOGGER = clone_object("/d/Imperial/bebop/sapho/obj/logger.c");

// Define a base mob level
// NOTE: This is not applied to the Sapho Junkies (../npc/junkie.c)
// They were meant to be frail.

int BASE_MOB_LEVEL = 120;

// Defefine a boss level modifier (adds an additional x% of base mob level to boss)
// Currently ~35%

float BOSS_MODIFIER = 0.45;

// Set the lifetime (in minutes) of a keycard

int KEYCARD_LIFETIME = 55;

// Set the value of the loot received from boxes (solaris)
// The transaction logic is handled by /room/den/den5.c

int SAPHO_LOOT_VALUE = 60000;

// Define chances of loot being found in the VIP area
// Currently ~2/5 - // used in /room/vip6.c & /room/vip8.c

int LOOT_CHANCE = 40;

// Return a random int between 0 and 100 that will decide
// the outcome of receiving loot from a box
// used in /room/vip6.c & /room/vip8.c

int generate_loot_outcome();

int generate_loot_outcome()
{
  int rand = random(100);
  return rand;
}


/* 
* Block players from moving until the bouncers are dead
* Also, if they don't have a keycard, kick em out of the
* VIP area. This function will override movement commands
* as well as the 'kill' and 'search' commands anywhere
* bouncers are found. 
* (/room/vip/vip2.c, vip6.c, vip7.c & vip8.c)
*/ 

int bounce_player(string str);

// this func takes the string args:
// "search", "move", or "kill"
// depending on the player's actions

int bounce_player(string str){
  
  object keycard;
  if (present("bouncer")){
    
    // Wizards only, bitch.
    if (wizardp(this_player())) {return 0;} 

    // Kick the player out if they don't have a keycard
    if (!(keycard = present("bebop_vip_keycard", this_player()))){
      // Add log message for when a player gets bounced
      LOGGER->writeLog(this_player()->query_name() + " got bounced from vip" 
      + (object_name(environment(this_player())) - "player/bebop/area/sapho/room/vip/") + " @ " + ctime(time()));

      write("\nNo keycard? Time to leave, chump!\n\n"
        +"A bouncer throws out of the VIP area and pulls a lever,\n"
        +"sending you falling into a pit!!\n\n");
      
      this_player()->move_player(
        "gets tossed out", ROOM_DIR + "deathpit.c");

      return 1;
    }

    // If they do have a keycard and bouncers are present..

    else {
      
      // If movement is attempted
      if (str == "move"){
        tell_object(this_player(),
          "The Bouncer asks: Where'd ya think you're goin'?\n");
        return 1;
      }
      
      // If a search is attempted...
      else if (str == "search"){ 
        tell_room(this_object(),
          "\nThe bouncer doesn't like " + this_player()->query_name() 
          + " digging around in the boxes.",({this_player()}));
        write(
          "The bouncer laughs and says: You should probably step away now.\n");
        return 1;

      }

      // If combat is attempted..
      else if (str == "kill"){return 0;}

    }
  }
  // Return 0 if no bouncers are present
  return 0;
}

