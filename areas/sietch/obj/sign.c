// sign.c - Living Sietch Warning Sign - Bebop 2021

#include <mudlib.h>
inherit OBJECT;
#include "/d/Fremen/bebop/sietch/include/defines.h"

string sign_string =""
      +"\n$BOLD$RED                          WARNING!$OFF\n\n"
      +"$BOLDThe mobs in this area are much bigger than in the northern area\n"
      +"                        (level $BOLD$RED" + BASE_MOB_LEVEL +"$WHITE+)\n\n"
      +"\n                 PROCEED AT YOUR OWN RISK!!\n\n"
      +"                           -Mgmt$OFF\n\n";

void reset(status arg)
{
  ::reset(arg);
  if (!arg) {
    set_name("sign");
    set_alias ( ({"sign"}) );
    set_short ("$BOLDA big $REDWARNING$WHITE sign$OFF");
    set_long(sign_string);
    
    set_no_get(1);
  }
}

void init ()
{
  ::init ();

  add_action ("do_read", "read");
}

int do_read (string str){
  if (str != "sign") {return notify_fail("Read what? The sign?\n");}

  tell_object(
      this_player(), sign_string);
    return 1;

}
