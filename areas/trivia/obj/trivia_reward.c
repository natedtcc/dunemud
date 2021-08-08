// trivia_reward.c - Rewards for winning trivia!
// Bebop 08/02/21

// TODO: Add everything

#include <mudlib.h>
inherit OBJECT;

string owner = this_player()->query_name();

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name ("reward");
    set_weight (0);
    set_short ("A Trivia Reward Voucher");
    set_alias( ({"reward", "voucher", "trivia_reward_voucher"}) );
    set_long ("This is a Trivia Reward voucher. You can use this to redeem one Trivia\n"
      +"reward. To use it, visit the Trivia Room and type 'redeem' for a list of prizes.\n");
  }
}

void init ()
{
  ::init ();
 
  add_action ("do_exchange", "exchange");

}

void do_exchange(){
  if (this_player()->query_name() != owner){
    return notify_fail("Sorry, this belongs to "+owner+"!");
  }
  else write("Lorem ipsum");
}