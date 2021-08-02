#include <mudlib.h>
inherit OBJECT;

/*
 * Nice work. Made a few changes a left some notes.
 *
 * Further reading:
 * man tell_object
 * man tell_room
 * man call_out
 * man destruct
 * man notify_fail
 */

int is_occupied = 0;


void reset(int arg)
{
  ::reset(arg);
  if(arg) return;

  set_name ("zong");
  set_alias ( ({ "bong", "zong", "tube" }) );
  set_weight (999);
  set_no_get (1);

  // remind me to show you sprintf and parse_ansi
  // makes color chunks much easier to read.

  set_short ("a$YELLOW 2' triple$OFF-$YELLOWkink "
  +"$REDbeer$OFF-$REDbottle $BROWNbrown $GREEN$BOLDZong$OFF");

  set_long ("\nShimmering against the light from above, this bong\n"
  +"almost seems heavenly. It's packed to the brim with some\n"
  +"sticky nug, topped with a bunch of kief. It looks delicious.\n"
  +"Do not take this bong lightly. It will. quite easily,\n"
  +"completely destroy your day and make you regret living..\n"
  +"You may 'rip' this Zong, at your own discretion.\n\n");
}

void init ()
{
  // what does the :: notation do, do you know?
  ::init ();

  add_action ("do_rip", "rip");
  add_action ("do_drop", "drop");
}

// changed the formatting a little
// showing a call_out chain
int do_rip (string str)
{
  if (!str || str !="zong")
  {
    // nice clean way to fail out
    return notify_fail("Rip what? The Zong?\n");
  }

  if (is_occupied){
    return notify_fail("The zong is currently in use.\n");
  }

  // what is TP and where is it defined?
  tell_object(TP, "You spark your lighter and begin to chalk the tube.\n");
  tell_room(environment(this_player()),
    sprintf("%s sparks %s lighter and begins to chalk a tube.\n",
    this_player()->query_name(),
    this_player()->query_possessive()),
    ({ this_player() }));
  is_occupied = 1;
  call_out("rip_message_two", 2);
  return 1;
}

int rip_message_two()
{
  tell_object(TP, "The $GREENZong$OFF becomes milky with smoke! You clear it!\n");
  call_out("rip_message_three", 2);
  return 1;
}

int rip_message_three()
{
  tell_object(TP, "You exhale a gigantic cloud of smoke and begin to cough your ass off.\n");
  call_out("clear_bowl", 1);
  return 1;
}

int clear_bowl()
{
  object ashtray;
  if ((ashtray = present("ashtray", environment(this_object())))){
    tell_room(environment(this_player()),
      sprintf("%s taps the hitter piece into the ashtray.\n",
      this_player()->query_name(),
      this_player()->query_possessive()),
      ({ this_player() }));
    tell_object(this_player(), "You tap the hitter piece out into the ashtray.\n");
    ashtray->add_hit();

  }
  is_occupied = 0;
  return 1;
}


/*
 * Instead of doing a call_out to destruct_this, you can simply move
 * those two statements directly into the body, like I've done here,
 * or you could just call destruct_this() directly. call outs are used
 * more for delayed execution, see what I did above with the zong rip
 * messages.
 */
int do_drop (string str)
{
  if (str=="zong" || str=="bong" || str=="tube")
  {
    //call_out ("destruct_this", 0);
    tell_object(this_player(),
                "As you drop your Zong, it shatters into tiny fragments.\n");

    // check out man sprintf!
    tell_room(environment(this_player()),
              sprintf("%s drops %s Zong and it shatters into tiny fragments.\n",
                      this_player()->query_name(),
                      this_player()->query_possessive()),
              ({ this_player() }));

    destruct(this_object());
    return 1;
  }
  return 0;
}
