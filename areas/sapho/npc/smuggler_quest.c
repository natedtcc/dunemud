// smuggler_quest.c  -  Sapho Smuggler quest mob - Bebop 2021

// This guy tells players about the quest, and accepts the quest
// item for quest completion.

#include <mudlib.h>
inherit "/obj/player/monster";
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

int cash_reward = 1500000;
int exp_reward = 2000000;

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {
      
    set_name("smuggler");
    set_alias( ({"smuggler", "questmob"}) );


    set_short("A Sapho Smuggler");
    set_long(
      "This smuggler is desperate to take over the red tar sapho trade. He\n"
      +"personally believes that if he were to somehow learn the recipe,\n"
      +"he could become one of the most powerful smugglers in the system.\n"
      +"Why don't you 'listen to smuggler' to hear what he has to say?");

    set_gender(1);
    set_race("fremen");
    set_level(10);
    set_non_combat_chats (10, ({
      ({ "Smuggler says: Won't you 'listen' to me?", "echo"}),
      ({ "Smuggler says: The recipe for red tar sapho will be mine!", "echo"}),
      ({ "Smuggler take a puff off of his spice spliff.", "echo"}),
      ({ "Smuggler says: Help me find that recipe!", "echo"}),
      ({ "Smuggler asks: Did you find it? Try to 'show recipe to' me!", "echo"}),
      ({ "Smuggler hrms." , "echo"}) }) );
    
  }
}

  void init()
  {
  ::init();

    add_action("listen_to", "listen");
    add_action("show_to", "show");
  }

  // Listen to the quest mob for a quest explaination

  int listen_to(string str){
    if (str != "to smuggler") return notify_fail("Listen to who? Smuggler??\n");

    write(
      "\nSmuggler says: Thanks for listening!\n"
      +"Smuggler says: I've been waiting down here for someone to come along! I\n"
      +"have been studying this facility and it's methods for some time, and\n"
      +"I finally believe I am ready to build an exact replica of it. The only\n"
      +"problem is that I don't have the exact recipe for creating this addicting\n"
      +"substance. I need someone to go find the recipe for me! I'm quite sure\n"
      +"this area's overseer, Gabriel, wouldn't let it far from his sight!\n\n"
      +"Smuggler says: Now please! Go find the recipe! I will reward you handsomely!\n");

    return 1;
  }

// Attempt to give the quest mob the quest item to complete the quest

int show_to(string str){
  object quest_item;
  if (str != "recipe to smuggler")
    return notify_fail("Show what to who??\n");
  
  else if (!(quest_item = present("recipe_quest_item", this_player())))
    return notify_fail("Smuggler says: You don't have the recipe!\n");
    
  else if (this_player()->query_quests("sapho_facility"))
    return notify_fail("You've already done this quest!\n");

  write(
    "\nSmuggler says: Excellent! This is it! Exactly what I've been after!\n"
    +"Smuggler lets out a rip roaring Muahahahahahahaha!!!\n"
    +"You've done well, friend. Here's your reward.\n"
    +"Smuggler takes the recipe from you and thanks you.\n"
    +"Smuggler gives you " + cash_reward + " solaris.\n"
    +"\nCongratulations! You completed the quest!\n\n"
    +"You feel more experienced.\n");

  LOGGER->writeLog(this_player()->query_name() + " completed the area quest" 
      + " @ " + ctime(time()));

  destruct(quest_item);
  this_player()->set_quest("sapho_facility");
  this_player()->add_money(cash_reward);
  this_player()->add_exp(exp_reward,"quest");
  this_player()->add_explorer_flag (object_name(environment(this_object())));

  return 1;
}