// Updated to actually function - Bebop 2021
inherit "/obj/object";
reset(arg) {
  ::reset(arg);
  if(arg)return;
  set_weight(1);
  set_name("box");
  set_alias("box");
  set_short("A strange box");
  set_long("Strange box that has strange markings on it...how fitting.\n\
  There is a keyhole in the center of the box.\n");
}

void init(){
  ::init();
  add_action("open", "open");
  add_action("unlock", "unlock");
}

/* This logic was originally defined in 
  /fate/room/cr1.c for whatever ridiculous
  reason. It was highly obfuscated and 
  totally impossible to figure out without
  reading the code. It's pretty funny actually:
      vcat /d/Tleilax/fate/room/cr1      */

int open(string str){
  if (str == "box"){
    return notify_fail("Try unlocking it.\n");
  }
  ::open(str);
  return 1;
}

int unlock(string str) {
  object room = clone_object("/d/Tleilax/fate/cr1.c");
  if(str == "box" && present("key",this(player)) && environment(this_player()) == room ) {
    if (present("/d/Tleilax/fate/obj/spice2.c", this_player())){
    write("Reacting to the light, the box unlocks!\n"
      +"You open it and find some liquid spice in it.\n"
      +"It is very pure.\n");
    move_object(clone_object("/d/Tleilax/fate/obj/spice2.c"), this_player());
    destruct(present("box", this_player()));
    destruct(present("key",this_player()));
    return 1;
    }
    else return notify_fail("You need the key..");
  }
else {
  return notify_fail("Unlock what???");
  }
}