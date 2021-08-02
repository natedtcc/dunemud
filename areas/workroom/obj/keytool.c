// keytool.c - Sapho Area Keycard Tool - Bebop 2021

#include <mudlib.h>
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";
inherit "/d/Imperial/bebop/sapho/include/keycard_gen.c";
inherit "/d/Imperial/bebop/sapho/include/lifetime.c";
inherit OBJECT;

object *keycards;

void reset(int arg) {
        ::reset(arg);
        if(arg)
                return;
        set_name("keytool");
        set_alias("keytool, finder");
        set_short("Keycard Finder");
        set_long("A tool for finding the keycard in the Sapho Facility.\n\n'key find'"
        +" - teleport to the currently spawned keycard.\n'key time' - get the time "
        +"remaining until despawn on the keycard.\n'key spawn' - spawn a keycard.\n"
        +"'key dest' - destroy the currently spawned keycard.\n\n");
        set_auto_load(1);
}

void init ()
{
  ::init ();
  add_action("do_key", "key");
}

int do_key(string str)
{ 
  keycards = clones(OBJ_DIR + "keycard.c");
  if (str == "find"){
    if (sizeof(keycards) > 0){
      write("Moving to current keycard location...\n");
      move_object(this_player(), object_name(environment(environment(keycards[0]))));
      return 1;
    }
  
    else return notify_fail("No keycards are currently spawned!\n");
  }
  
  if (str == "spawn"){
    if (sizeof(keycards) < 1){
      keycard_gen();
      write("Keycard generated.\n");
      return 1;
    }
    else return notify_fail("Keycard already spawned!\n");
  }
    if (str == "time"){
      if (sizeof(keycards) > 0){
        int lifespan_time = keycards[0]->query_lifespan_time() - time();
        write("Time remaining until keycard respawn:\n" 
          + (lifespan_time / 60 )
          + " minutes and " 
          + (lifespan_time % 60)
          + " seconds.\n");
        return 1;
      }

    else return notify_fail("No keycard currently spawned!\n");
  }
    if(str == "dest"){
      object keycard;
      if (sizeof(keycards) > 0){
        keycard = keycards[0];
        write("Keycard destroyed.\n");
        destruct(keycard);
        return 1;
    }

    else return notify_fail("No keycard currently spawned!\n");
  }

  else return notify_fail("'examine keytool' for more info.\n");
}

