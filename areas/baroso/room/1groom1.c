/* 
** Guard Room 
** by Kiza 24-Aug-1996
*/

inherit "/room/newroom";
inherit "/d/Fremen/bebop/sietch/include/area_utils.c";

void reset(int arg) {
  ::reset(arg);
  
  if(!present("guard",this_object())) {
    transfer(clone_object("/d/Atreid/kiza/ubase/npc/vet_guard.c"),this_object());
  }

  if(arg) return;
  
  set_light(1);
  short_desc = "Baroso Guard's Room";
  long_desc = 
"This room is rather small, and is used to house the Baroso Marine Guards\n"+
"the main security for Baroso.  The room is finely decorated, with a small\n"+
"desk in one corner, with a bed next to it.  Above the desk is a book shelf\n"+
"filled with all sorts of fine reading literature.\n\n";

  dest_dir = ([
    "south":"/d/Atreid/kiza/ubase/ghall1.c",
  ]);
 
  items = ([
    "desk":
"This is a finely crafted work desk, used by the guard who resides in this\n"+
"room, it has neatly stacked papers and books in each corner and looks well\n"+
"used by the rooms resident.\n",
    "bed":
"This is a small, but extremely comfortable looking bed which has been neatly\n"+
"made, the covers have a small hawk emblem in the corner, signifying they\n"+
"belong to house Atreides\n",
    "shelf":
"On the shelves are many fine books which have been published over the years.\n",
    "shelves":"=shelf",
    "books":"=shelf",
    "book shelf":"=shelf",
  ]);
  
 
  return;
}

