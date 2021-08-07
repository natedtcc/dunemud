// procworker.c - Mobs in the sapho processing area - Bebop 2020

inherit "/obj/player/monster";

void reset(status arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("tester");
    set_race("none");
    set_gender(1);
    set_short("Scaling Tester");
    set_long("Just a lil' test mob.");
  }

}

void init()
{
  ::init();

  add_action("kill", "kill");
  add_action("consider", "consider");
}

int kill (string arg){
  if (!query_attack() 
    && environment(this_object())->query_occupied <= 2
    && arg == lower_case(this_object()->query_name()))    
      set_level(this_player()->query_level() + random(5));    
  return 0;
}

int consider (string arg){
  if (!query_attack() 
    && environment(this_object())->query_occupied <= 2
    && arg == lower_case(this_object()->query_name()))    
      set_level(this_player()->query_level() + random(5));    
  return 0;
}