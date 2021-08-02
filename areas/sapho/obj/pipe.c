// pipe.c - Smokable red-tar sapho pipe - Bebop 2018

// TODO: Add num_hits, color algo

#include <mudlib.h>
inherit OBJECT;

// Declare vars for handling of packing/smoking
int packed = 0, smoking = 0, num_hits = 0;
string pipe_desc = "This pipe is long and narrow, with an aggressive bend about\n"
    +"halfway down the neck. It looks somewhat like a Sherlock. Perhaps if\n"
    +"you had some sapho to 'pack', you could 'smoke' this pipe..\n";

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name ("pipe");
    set_weight (0);
    set_short ("A Sapho Pipe");
    set_long (pipe_desc);
  }
}

void init ()
{
  ::init ();
 
  add_action ("do_pack", "pack");
  add_action ("do_smoke", "smoke");
}

// Pack that shit (unless its already packed or you're already smoking)
int do_pack (string str)
{  
  object smokable;

  if(str != "sapho"){return notify_fail("Pack what? Sapho? The pipe?\n");}

  else if (smoking) {
    return notify_fail("You can't pack your pipe while you're smoking!\n");
  }

  else if (packed){
    return notify_fail("You've already packed your pipe!\n");
  }

  else if (!(smokable = present("sapho_smokable", this_player()))){
	  return notify_fail("You don't have any sapho!\n");
  }
  
  packed = 1;
  num_hits = 4;
  write("You pack your sapho pipe.\n");
  tell_room(environment(this_player()),
    sprintf("%s packs %s pipe with a chunk of sapho.\n",
    this_player()->query_name(),
    this_player()->query_possessive()),
    ({ this_player() }));
  // Dest the sapho in inventory
  destruct(smokable);
  set_long (pipe_desc + "There are currently " + num_hits + " hits left in the pipe.\n");

  return 1;
}

// Smoke that shit (unless you already are, or haven't packed it)
int do_smoke (string str)
{
  if (str != "pipe") {return notify_fail("Smoke what? The pipe?\n");}
  
  else if (!packed) {return notify_fail("You have not packed your pipe!\n");}

  else if (smoking) {return notify_fail("You are already smoking your sapho pipe!\n");}

  smoking = 1;
  tell_object(this_player(),
    "You light your lighter and take a long puff off your sapho pipe.\n");
  tell_room(environment(this_player()),
    sprintf("%s takes a huge rip off their sapho pipe!\n",
    this_player()->query_name(),
    this_player()->query_possessive()),
    ({ this_player() }));
  
  num_hits--;
  // Remove plural keywords from desc (single hit remains)
  if (num_hits == 1){
    set_long (pipe_desc + "There is currently " + num_hits + " hit left in the pipe.\n");
  }

  // Revert to basic desc (empty pipe)
  if (num_hits == 0){ 
    packed = 0;
    set_long(pipe_desc);
  }

  // Otherwise set the desc to show num_hits > 1
  else set_long (pipe_desc + "There are currently " + num_hits + " hits left in the pipe.\n");
  call_out("do_smoke_2", 4);
  return 1;
}

int do_smoke_2()
{
  tell_object(this_player(),
    "You exhale slowly. Your whole body feels incredible. You take another"
	+" toke.\n");
  call_out("do_smoke_3", 4);
  return 1;
}

int do_smoke_3()
{
  tell_object(this_player(),
    "You breathe out deeply. You feel like you are floating on a cloud.\n"
	+"You feel a slight lapse of conciousness coming on...\n");
  call_out("trippin_balls", 4);
  return 1;
}

int trippin_balls()
{
	
  tell_object(this_player(),
    "\n\n$BOLD$RED               __,aaPPPPPPPPaa,__\n"
    +"             ,adP\"\"\"'          `\"\"Yb,_\n"
    +"          ,ad\"'                     `\"Yb,\n"
    +"        ,dP'     _,adPP\"\"\"\"\"YYba,_     `\"Y,\n"
    +"       aP'     ,d\"\"'           `\"\"Ya,     \"Y,\n"
    +"     ,d\"     ,P\"     _________     `\"b,    `Yb,\n"
    +"    ,d'     d\"    ,adP\"\"\"\"\"\"\"\"Yba,    \"b,    \"Y,\n"
    +"    d'    ,P'   ,dP\"            `Yb,   `Y,    `Y,\n"
    +"    8    ,P'   ,d'    ,dP\"\"Yb,    `Y,   `Y,    `b\n"
    +"    8    d'    d'   ,d\"      \"b,   `Y,   `8,    Y,\n"
    +"    8    8     8    d'    _   `Y,   `8    `8    `b\n"
    +"    8    8     8    8     8    `b    8     8     8\n"
    +"    8    Y,    Y,   `b, ,aP     P    8    ,P     8\n"
    +"    I,   `Y,   `8,    \"\"\"\"     d'   ,P    d\"    ,P\n"
    +"    `b,   `8,    \"b,         ,P\"   ,P'   ,P'    d'\n"
    +"     `b,   `Ya,    \"Ya,,__,aP\"    ,P'   ,P\"    ,P\n"
    +"      `Y,    `Ya,     `\"\"\"''     ,P'   ,d\"    ,P'\n"
    +"       `Yb,    `\"Ya,_         _,d\"    ,P'    ,P'\n"
    +"         `Yb,      \"\"YbaaaaadP\"'     ,P'    ,P'\n"
    +"           `Yba,                   ,d'    ,dP'\n"
    +"              `\"Yba,__       __,adP\"     dP'\n"
    +"                  `\"\"\"\"\"\"\"\"\"\"\"\"\"'\n$OFF"
    +"\nYou're trippin' balls, dude!!\nYou took too much,"
    +" man. You took too much, too much!\n");
    
    
    smoking = 0;
    return 1;
}
