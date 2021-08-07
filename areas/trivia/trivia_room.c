// trivia_room.c - a room for Trivia!
// Original by Sauron 12/99
// Updated by Bebop 08/01/2021

inherit "/room/newroom";
#define TRIVIA_NPC "/d/Atreid/domain/trivia/trivia"

int is_spawning = 0;

reset(arg)
{
  if(arg)return;
  set_light(1);
  set_property("no_fight", 1);
  set_property("no_rndmove");
  set_planet("caladan");
  set_indoor();
  short_desc = "DuneMUD Trivia Room";
  long_desc = "\
Welcome to the DuneMUD Trivia Room. This is where trivia takes\n\
place. There's a large chute above you, with a hatch that's sealed\n\
up. For more information on trivia, see 'help trivia'.\n\
\nTo start the Trivia countdown, simply type 'start'.\n\
NOTE: As a courtesy to others, please disable your triggers when a game\n\
is in session. Enjoy the game!\n";
  dest_dir = ([
    "down":"d/Space/stryder/planets/cal_station",
  ]);
  items = ([
    "chute":"A chute. It's pretty big.",
    "hatch":"It's latched shut.",
  ]);
}

init()
{
  ::init();
  call_out("welcome_message", 1);
  add_action("trivia_start", "start");
}

// Start a game of trivia

int trivia_start(string str){

  // if (query_occupied() < 5){
  //   return notify_fail("Sorry, you need 3 or more people to start Trivia!\n");
  // }

  // if (!wizardp(this_player())){
  //   return notify_fail("What ?\n");
  // }

  if (!str || str != "trivia"){
    return notify_fail("To start Trivia, type 'start trivia'.\n");
  }

  // Load the Trivia NPC, move it to the current wizard, and initialize it

  else if (is_spawning){
    return notify_fail("A TriviaBot is currently spawning...\n");
  }

  else {
    if (!present("trivia_npc", this_object())){
      tell_room(this_object(), parse_ansi(
        "\nYou notice a rumbling from the chute above you ...\n\n"));
      is_spawning = 1;
      call_out("start_trivia", 2);
        }
    else {
      return notify_fail("A TriviaBot is already present!\n");
    }

  return 1;
  }

}

void welcome_message(){
  if (wizardp(this_player())){
    tell_object(this_player(), "Welcome to the new Trivia room, " + this_player()->query_name()
    +"!\nTo start a new game of Trivia, type 'trivia start'.\n");
  }
}

// Move the TriviaBot to the room and call it's starup function
void start_trivia(){
  tell_room(this_object(), parse_ansi(
  "\nYou hear a loud $BOLD$YELLOWbeep$OFF as a $BOLD$REDTriviaBot$OFF"
  +" drops in from the chute\nabove, and begins it's "
  +"$UNDERL$BOLD$WHITEs$CYANt$WHITEa$CYANr$WHITEt$CYANu$WHITEp$OFF "
  +"$UNDERL$BOLDs$CYANe$WHITEq$CYANu$WHITEe$CYANn$WHITEc$CYANe$WHITE$OFF.\n\n"));
  is_spawning = 0;
  move_object(TRIVIA_NPC, this_object());
  call_other(TRIVIA_NPC, "startup");
}