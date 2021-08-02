inherit "/room/newroom";
reset(arg) {
  if(arg)return;
  set_light(0);
  short_desc = "Secret Hallway";
  long_desc = "\
This is the end of the secret hallway, yet\n\
all it is is a dead end. There must be some-\n\
place you can go, otherwise, you've just wasted\n\
a whole bunch of time. You notice something odd\n\
about a wall here.\n";
  dest_dir = ([
    "west":"/d/Tleilax/fate/rooms/shallway2.c",
  ]);
  items = ([
    "wall":"There is something amiss, like many of the walls in this castle.",
  ]);
}
init() {
  ::init();
  add_action("search","search");
  add_action("push","push");
}
search(str) {
  if(!str) {
    write("Search what???\n");
    return 1;
  }
  if(str == "wall") {
   write("There is a loose block near the bottom of the wall...\n");
   return 1;
  }
  else write("Search what???\n");
  return 1;
}
push(str) {
  if(!str) {
   write("Push what???\n");
   return 1;
  }
  if(str == "block") {
    write("A mysterious door opens and it sucks you in!\n");
    this_player()->move_player("You can't resist it and get pulled through.",
                               "/d/Tleilax/fate/rooms/duncan.c");
  return 1;
}
  else write("Push what???\n");
  return 1;
}

