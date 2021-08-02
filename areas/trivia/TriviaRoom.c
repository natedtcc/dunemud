// TriviaRoom.c
// Room for playing Dune Trivia
// Sauron
// 12/99

inherit "/room/newroom";

reset(arg)
{
  if(arg)return;
  set_light(1);
  set_property("no_fight", 1);
  set_property("no_rndmove");
  set_planet("caladan");
  set_indoor();
  short_desc = "Dune Trivia Room";
  long_desc = "\
This is the room for playing Dune Trivia.\n\
Please shut off all triggers and keep conversation\n\
to a minimum because it gets confusing with 10 or\n\
more people in here.\n";
  dest_dir = ([
    "down":"d/Space/stryder/planets/cal_station",
  ]);
}

init()
{
  ::init();
  add_action("illegal","grep");
}

illegal(string str)
{
  write("Not in this room!\n");
  return 1;
}

void init(){
  ::init();

    add_action("call_mentat","call");
    add_action("do_climb","climb");
}

int call_mentat(string str){
  if (!this_player()->wizardp()){
    return notify_fail("What ?")
  }

  else 
    if (str == "mentat" && !present()){
      tell_room(this_object(), this_player()->query_name()
      +" calls for the Trivial Pursuit mentat.\n");
    }
}