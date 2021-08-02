inherit "/room/newroom";
int SPICE=1;
int BOX,KEY;
int DESK=0;
int HUM=0;
reset(arg) {
  HUM=0;
if(!present("ghola")) {
  move_object(clone_object("/d/Tleilax/fate/npc/mghola.c"), this_object());
}
  if(arg)return;
  set_light(1);
  short_desc = "Ghola's hideout!";
  long_desc = "\
You have entered the room of a runaway ghola.\n\
It seems that this special ghola somehow got\n\
free of its master's bind and got some tiny\n\
little bit of brain matter. You should beware\n\
of it's aggressive nature. I don't think it\n\
wants to be found. There is an odd light shining\n
in the center of this room..\n";
  items = ([
    "room":"There is a crude, large stone desk in the corner of the room.",
    "desk":"The desk seems to be made from the cavern wall!",
    "wall":"There are some strange markings on it.",
    "markings":"They are foreign symbols to you. If you had some kind of symbol decoder,
    though...",
  ]);
  dest_dir = ([
    "south":"/d/Tleilax/fate/rooms/cavern.c",
  ]);
}

/* The box can only be open from this room (as per Fate's design)
   so I moved the box logic into the actual box and added a check
   there to make sure it can only be open in this room. Also the
   player will get a hint message when they enter this room with
   the box. Also, the below code ... ffear. Talk about an impossible
   quest. Pretty sure it doesn't work - but meh :)
                                              - Bebop, July 2021*/

init() {
  ::init();
  if (present("box", this_player()) && !HUM){
    HUM = 1;
    write("Your box begins to hum as you enter this room.\n");
  }
  //add_action("enter","enter");
  //add_action("insert","insert");
  add_action("decode","decode");
  //add_action("search","search");
  //add_action("unlock","unlock");
}
decode(str) {
  if(!str) {
   write("Decode what???\n");
   return 1;
  }
  if(str == "symbols") {
    object boxx;
    boxx = present("box", this_player());
    if(!boxx) {
     write("And how do you plan on decoding this you stupid fool???\n");
     return 1;
    }
    write("\
The markings on the wall suddenly form\n\
into words that you understand. It says:\n\
BEWARE HE WHO BELIEVES IS TRUE,\n\
THE CODE HE ENTERS TO BE THE ONE,\n\
FOR IT MAY BE THE GIVER OF LIFE,\n\
OR THE DESTROYER OF THE PRODIGAL SON.\n");
  BOX=1;
  return 1;
  }
  else write("Decode what???\n");
  return 1;
}

enter(str) {
  if(!str) {
   write("Enter what???\n");
   return 1;
  }
  if(!BOX) {
   write("You cannot enter.\n");
   return 1;
  }
  if(str == "money") {
    write("Something in the desk opens.\n");
    DESK = 1;
    return 1;
  }
  else write("Enter what???\n");
  return 1;
}
search(str) {
  if(!str) {
   write("Search what???\n");
   return 1;
  }
  if(str == "desk") {
    if(DESK == 1 && present("box", this_player())) {
    write("You find a locked box belted to an open compartment in the desk.\n"+
          "You may be able to unlock it with some special key.\n");
    KEY = 1;
    return 1;
    }
  else if(DESK == 0) {
  write("Search what???\n");
  return 1;
  }
  else write("Search what???\n");
  if(!str) {
   write("Insert what into what???\n");
   return 1;
  }
  if(!KEY){
    write("What??\n");
    return 1;
  }
  if(str == "key into box") {
   object keyy;
   keyy = present("keyy", this_player());
   if(!KEY) {
    write("Insert what???\n");
    return 1;
   }
   else if(present("key", this_player()) && KEY == 1) {
    write("You insert the key into the box.\n");
    BOX = 1;
    return 1;
  }
  else if(KEY == 0);
  write("Insert what into what???\n");
  return 1;
  }
  else write("Insert what into what???\n");
  return 1;
}
unlock(str) {
  if(!str) {
   write("Unlock what???\n");
   return 1;
  }
  if(str == "box") {
    if(BOX == 0) {
     write("Unlock what???\n");
     return 1;
     }
    if(BOX == 1 && SPICE == 0) {
     write("The spice has already been taken!\n");
     return 1;
    }
    if(BOX == 1 && SPICE == 1 && KEY == 1 && present("box", this_player()) && present("key",
    this_player())) {
     write("You unlock the box and find some liquid spice in it. It is very pure.\n");
     move_object(clone_object("/d/Tleilax/fate/obj/spice2.c"), this_player());
     SPICE = 0;
     BOX = 0;
     KEY = 0;
     DESK = 0;
     destruct(present("box", this_player()));
     destruct(present("key",this_player()));
     return 1;
    }
  write("Unlock what???\n");
  return 1;
  }
else write("Unlock what???\n");
return 1;
}
