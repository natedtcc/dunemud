   1 /*
   2 **  obj/cmds/player/tell.c
   3 **
   4 **  MUD-wide tell command.
   5 **  Iria@Dune.
   6 */
   7
   8 #include <mudlib.h>
   9 #include <player/login.h>
  10 #include <udp.h>
  11
  12 inherit CMD;
  13
  14 #define SHUT_OBJ        "obj/npc/shut"
  15 #define TRIVIA_OBJ      "d/Atreid/domain/trivia/trivia"
  16 #define SAVE_TELL       "/data/daemons/tell_data"
  17
  18 //#define TELL_SP_COST    3
  19 #define TELL_SP_COST    0
  20 #define MAX_HISTORY     50
  21 #define VAR_BLCK        "block_tells"
  22
  23 nomask mapping tell_history;
  24 nosave mapping tell_handlers;
  25
  26 void reset (int arg)
  27 {
  28   if (arg)
  29   {
  30 #ifdef SAVE_TELL
  31     save_object (SAVE_TELL);
  32 #endif // SAVE_TELL
  33   }
  34   else
  35   {
  36     aliases = ([ "reply" : "obj/cmds/player/reply",
  37                  "htell" : "obj/cmds/player/htell"
  38                ]);
  39
  40     tell_history = ([ ]);
  41 #ifdef SAVE_TELL
  42     restore_object(SAVE_TELL);
  43 #endif // SAVE_TELL
  44   }
  45   tell_history = filter (tell_history, #'user_exists);
  46   tell_handlers = ([ ]);
  47 }
  48
  49 #ifdef SAVE_TELL
  50 void destructor ()      {  save_object (SAVE_TELL);     }
  51 #endif // SAVE_TELL
  52
  53 string *query_tell_history(string user) {
  54   if (!this_player() || (this_player()->query_real_name() != user && !baronp(this_player())))
  55     return 0;
  56
  57   return (user && member(tell_history, user)) ? tell_history[user] : 0;
  58 }
  59
  60 status remove_tell_history (string user) {
  61   if (!this_player() || (this_player()->query_real_name() != user && !baronp(this_player())))
  62     return 0;
  63
  64   if (user && member(tell_history, user))
  65     m_delete(tell_history, user);
  66
  67   return 1;
  68 }
  69
  70 void mkhistory(mixed me, mixed from, string message, status is_reply) {
  71   if (extern_call() && strstr(regreplace(object_name(previous_object()), "^/*", "/", 0), "/secure/udp/"))
  72     return;
  73
  74   if (!(objectp(me)   || stringp(me))   ||
  75       !(objectp(from) || stringp(from)) ||
  76       !stringp(message))
  77     return;
  78
  79   if (objectp(me) && playerp(me) && !guestp(me)) {
  80     string  me_name     = (string)(me->("query_real_name")());
  81     string  from_name   = stringp(from) ? from : playerp(from) ? (string)(from->query_real_name()) : (string)(from->
    query_name());
  82
  83     if (!member(tell_history, me_name))
  84       tell_history += ([ me_name: ({ }), ]);
  85
  86     tell_history[me_name] += ({
  87       sprintf("[%s]: %s %s: %s",
  88         ctime(user_time(me))[0..15],
 /obj/cmds/player/tell.c 88/493 :   89         capitalize(from_name),
  90         is_reply ? "replies" : "tells you",
  91         message),
  92     });
  93
  94     if (sizeof(tell_history[me_name]) > MAX_HISTORY)
  95       tell_history[me_name] = tell_history[me_name][<MAX_HISTORY..];
  96   }
  97
  98   if (!stringp(from) && playerp(from) && !guestp(from)) {
  99     string  me_name     = stringp(me) ? me : playerp(me) ? (string)(me->query_real_name()) : (string)(me->query_name()
    );
 100     string  from_name   = (string)(from->query_real_name());
 101
 102     if (!member(tell_history, from_name))
 103       tell_history += ([ from_name: ({ }), ]);
 104
 105     tell_history[from_name] += ({
 106       sprintf("[%s]: You %s %s: %s",
 107         ctime(user_time(from))[0 .. 15],
 108         is_reply ? "reply to" : "tell",
 109         capitalize(me_name),
 110         message),
 /obj/cmds/player/tell.c 110/493 :  111     });
 112
 113     if (sizeof(tell_history[from_name]) > MAX_HISTORY)
 114       tell_history[from_name] = tell_history[from_name][<MAX_HISTORY..];
 115   }
 116 }
 117
 118 string* query_tell_handlers() { return m_indices(tell_handlers); }
 119
 120 int add_tell_handler(string tell_name, object tell_handler) {
 121   if (!(stringp(tell_name) && sizeof(tell_name) && !sizeof(regreplace(tell_name, "^[a-z]+$", "", 0))) ||
 122       (file_size(USER_BANISH(tell_name) +".o") < 0) ||
 123       !(objectp(tell_handler) && stringp(tell_handler->query_name())) ||
 124       0)
 125     return 0;
 126
 127   tell_handlers[tell_name] = tell_handler;
 128
 129   return 1;
 130 }
 131
 132 int remove_tell_handler(string tell_name) {
 /obj/cmds/player/tell.c 132/493 :  133   if (member(tell_handlers, tell_name)) {
 134     m_delete(tell_handlers, tell_name);
 135
 136     return 1;
 137   }
 138
 139   return 0;
 140 }
 141 private status valid_tell (object to, object from)
 142 {
 143   int no = 0;  // made readable by Stryder 100101
 144
 145   if (!to)     // no person to say to
 146     return 0;
 147
 148   if (to->query_wizard_invis () &&
 149       to->defined_wiz_env_var ("no_tell") &&
 150       from->QWL <= to->query_wizard_invis())
 151     no = 1;
 152
 153   if (to->query_env_var ("ignored_people") && // OK
 154       member (explode (to->query_env_var ("ignored_people"), "#"), from->query_real_name ()) != -1)
 /obj/cmds/player/tell.c 154/493 :  155     no = 1;
 156
 157   if (to->defined_env_var ("whitelist") &&
 158       !wizardp (from) &&
 159       intersect (explode (to->query_env_var ("whitelist"), "#"),
 160                  ({ "all", from->query_real_name () })) == ({ }))
 161     no = 1;
 162
 163   if (!from->can_see (to, 1))
 164     no = 1;
 165   else
 166     no = 0;
 167
 168   if (to->defined_env_var ("whitelist") &&
 169       intersect (explode (to->query_env_var ("whitelist"), "#"),
 170                  ({ "all", from->query_real_name () })) != ({ }))
 171     no = 0;
 172
 173   if (from->query_wiz_level () > to->query_wiz_level ())  // You can always tell to someone with a lower wizlevel.
 174     no = 0;
 175
 176   return !no;
 /obj/cmds/player/tell.c 176/493 :  177 }
 178
 179 private void htell ()
 180 {
 181   if (guestp (this_player()) || !member (tell_history, this_player()->query_real_name()))
 182   {
 183     write ("No history.\n");
 184     return;
 185   }
 186
 187   printf ("%sHistory tell :%s\n%s\n",
 188         ACOL ("bold"), ACOL ("normal"), implode (tell_history [this_player()->query_real_name()], "\n"));
 189 }
 190
 191 status main (string str)
 192 {
 193   object ob;
 194   mixed *us, *npcs;
 195   status is_reply;
 196   string col, who, user, mudname, msg, *tmp;
 197
 198   if (!command_allowed (str))
 /obj/cmds/player/tell.c 198/493 :  199     return 1;
 200   is_reply = (query_verb () == "reply");
 201   if (!is_reply && !str)
 202   {
 203     htell ();
 204     return 1;
 205   }
 206
 207 #ifdef VAR_BLCK
 208   if (!is_reply && sscanf (str,"block %s", msg) == 1)
 209   {
 210     msg = lower_case (trim (msg));
 211     switch (msg)
 212     {
 213       case "on":
 214         this_player()->set_env_var (VAR_BLCK, "on");
 215         write ("You are now blocking all tells from players lower level than you.\n");
 216         return 1;
 217       case "off":
 218         if (this_player()->defined_env_var (VAR_BLCK) &&
 219             lower_case (this_player()->query_env_var (VAR_BLCK)) != "off")
 220         {
 /obj/cmds/player/tell.c 220/493 :  221           this_player()->delete_env_var (VAR_BLCK);
 222           write ("You are no longer blocking tells from anyone.\n");
 223         }
 224         else
 225           write("You were not blocking tells.\n");
 226         return 1;
 227       case "all":
 228         this_player()->set_env_var (VAR_BLCK, "all");
 229         write ("You are now blocking tells from everyone.\n");
 230         return 1;
 231       default:
 232         tmp = filter (explode (msg, " ") - ({ "" }), #'user_exists);
 233         if (!sizeof (tmp))
 234         {
 235           write ("No one to tell block from!\n");
 236           return 1;
 237         }
 238         msg =_implode (map (tmp, #'capitalize), ", ", " and ");
 239         this_player()->set_env_var (VAR_BLCK, msg);
 240         printf ("You are now blocking tells from %s.\n", msg);
 241         return 1;
 242     }
 /obj/cmds/player/tell.c 242/493 :  243   }
 244
 245   if (str == "block")
 246   {
 247     if (this_player()->defined_env_var (VAR_BLCK) &&
 248         lower_case (str = this_player()->query_env_var (VAR_BLCK)) != "off")
 249     {
 250       printf("Your tell block is currently set to : %s.\n",str);
 251       return 1;
 252     }
 253     write("You are not blocking tells from anyone.\n");
 254     return 1;
 255   }
 256 #endif // VAR_BLCK
 257
 258   if (this_player()->query_ghost ())
 259   {
 260     write("You fail.\n");
 261     return 1;
 262   }
 263   if (!wizardp (this_player()) &&
 264       (find_player (explode (str, " ")[0]) && !wizardp (find_player (explode (str, " ")[0]))) &&
 /obj/cmds/player/tell.c 264/493 :  265       this_player()->query_cp () <= TELL_SP_COST)
 266   {
 267     printf ("You are too tired to '%s' right now.\n",query_verb());
 268     return 1;
 269   }
 270
 271   if (sscanf (str, "%s %s", who, msg) != 2)
 272   {
 273     write("\
 274 USAGE: tell <user[@mudname]> <message>\n\
 275        tell block <on|all|name1 [name2 ... nameN]>\n");
 276         return 1;
 277   }
 278
 279   if (sscanf (who, "%s@%s", user, mudname) == 2)
 280   {
 281     user = lower_case (user);
 282 //  if (lower_case (mudname) != lower_case (LOCAL_NAME))
 283 //  {
 284       if (str = INETD->send_data (mudname,
 285                                   ([
 286                                     REQUEST : "tell",
 /obj/cmds/player/tell.c 286/493 :  287                                     RECIPIENT : user,
 288                                     SENDER : this_player()->query_real_name(),
 289                                     DATA : sprintf ("%s\n", msg),
 290                                     "is_reply" : is_reply,
 291                                    ]), 1))
 292         write(str);
 293       else
 294         write("Message transmitted.\n");
 295
 296       if (!wizardp (this_player()))
 297         this_player()->add_cp (-TELL_SP_COST);
 298       mkhistory (sprintf ("%s@%s", user, capitalize (mudname)), this_player(), msg, is_reply);
 299       return 1;
 300 //  }
 301 //  else
 302 //    who = user;
 303   }
 304
 305   if (objectp(tell_handlers[who]) && tell_handlers[who]->hear_tell(this_player(), msg, is_reply))
 306     ob = tell_handlers[who];
 307   else {
 308     string err;
 /obj/cmds/player/tell.c 308/493 :  309     status bypass;
 310
 311     who = lower_case (who);
 312     if (find_player (who))
 313       us = ({ find_player (who) });
 314     else
 315       us = filter (heart_beat_info (), #'playerp);
 316
 317     err = catch (us = regexp (map_objects (filter (us, #'valid_tell, this_player()), "query_real_name"),
 318                               sprintf ("^%s", who)); nolog);
 319     if (err)
 320       us = ({ });
 321
 322     if (!valid_tell (find_player (who), this_player ()))
 323     {
 324       object whom = find_player (who);
 325
 326       if (whom && this_player ()->can_see (whom))
 327       {
 328         write ("That player is not listening to you.\n");
 329         return 1;
 330       }
 331
 332       if (!find_player (who) || !this_player ()->can_see (find_player (who)))
 333       {
 334         write ("There is nobody by that name to communicate with.\n");
 335         return 1;
 336       }
 337     }
 338
 339     if (sizeof (us) > 1 && !find_player (who))
 340     {
 341       printf ("Tell who ? %s ?\n", _implode (map (us, #'capitalize), ", ", " or "));
 342       return 1;
 343     }
 344
 345     if (!ob = find_player (us [0]))
 346     {
 347       write ("No player with that name.\n");
 348       return 1;
 349     }
 350
 351     if (this_player() == ob)
 352     {
 /obj/cmds/player/tell.c 352/493 :  353       write ("Tell something to yourself ? Hmmm...\n");
 354       return 1;
 355     }
 356
 357     if (!wizardp(this_player()) && !wizardp (ob))
 358       this_player()->add_cp (-TELL_SP_COST);
 359
 360     if (ob->query_busy () &&
 361         ((wizardp (ob) && this_player()->QWL < ob->QWL) || this_player()->QL < ob->QL))
 362     {
 363       write ("That person is busy right now. Please try again later.\n");
 364       return 1;
 365     }
 366
 367 #ifdef VAR_BLCK
 368     //Orbital - Apr 26/2010
 369     //block_tells           Keorode, Mechaks and Argus
 370     if(this_player()->defined_env_var (VAR_BLCK))
 371     {
 372       tmp = explode(this_player()->query_env_var(VAR_BLCK)," ");
 373       foreach(string s : tmp)
 374       {
 /obj/cmds/player/tell.c 374/493 :  375         if(member(explode(s,","),capitalize(ob->query_real_name())) != -1)
 376         {
 377           write("You cannot sent a tell to someone you are currently blocking.\n");
 378           return 1;
 379         }
 380       }
 381     }
 382     //Orbital - Apr 26/2010
 383     //ignored_people        keorode#mechaks#argus
 384     if(this_player()->defined_env_var ("ignored_people"))
 385     {
 386       tmp = explode(this_player()->query_env_var("ignored_people"),"#");
 387       if(member(tmp,ob->query_real_name()) != -1)
 388       {
 389         write ("You cannot send a tell to someone you are currently blocking.\n");
 390         return 1;
 391       }
 392     }
 393
 394     bypass = (wizardp (this_player()) && this_player()->QWL >= ob->QWL);
 395     if (ob->defined_env_var (VAR_BLCK) && !bypass)
 396     {
 /obj/cmds/player/tell.c 396/493 :  397       user = ob->query_env_var (VAR_BLCK);
 398       if (user == "on" &&
 399           (!bypass && ((wizardp(ob) && this_player()->QWL<ob->QWL) ||
 400           this_player()->QL < ob->QL)))
 401       {
 402         write ("That person is blocking tells from users below his level.\n");
 403         return 1;
 404       }
 405
 406       if (user == "all")
 407       {
 408         write ("That person is blocking all tells.\n");
 409         return 1;
 410       }
 411
 412       tmp = map (explode (user, " ") - ({ "" }), #'lower_case);
 413       if (member (tmp, this_player()->query_real_name()) != -1)
 414       {
 415         write ("That person is blocking tells from you.\n");
 416         return 1;
 417       }
 418     }
 /obj/cmds/player/tell.c 418/493 :  419 #endif // VAR_BLCK
 420
 421     if (this_player()->query_away () != "" || !this_player()->query_away ())
 422       printf ("%s[Warning]%s You are set as away (%s).\n",
 423               ACOL ("bold red"), ACOL("normal"), this_player()->query_away ());
 424
 425     if (ob->query_ghost ())
 426       printf("%s[Warning]:%s %s is dead.\n",
 427              ACOL ("bold red"), ACOL ("normal"), capitalize (ob->query_real_name ()));
 428     else if (interactive (ob))
 429     {
 430       if (query_idle (ob) > 300)
 431         printf ("%s[Warning]:%s this player has been idle for %s.\n",
 432                 ACOL ("bold red"), ACOL ("normal"), format_time (query_idle (ob), 1));
 433       else if (query_editing (ob))
 434         printf ("%s[Warning]:%s %s is editing.\n",
 435                 ACOL ("bold red"), ACOL ("normal"), capitalize (ob->query_real_name()));
 436     }
 437     else if (!interactive (ob))
 438       printf("%s[Warning]:%s %s is link-dead.\n",
 439              ACOL ("bold red"), ACOL ("normal"), capitalize (ob->query_real_name ()));
 440
 /obj/cmds/player/tell.c 440/493 :  441     ob->set_attribute("reply_to",this_player()->query_real_name());
 442   }
 443
 444   mkhistory(ob, this_player(), msg, is_reply);
 445   col = this_player()->defined_env_var ("tell_to_colour") ? this_player()->query_env_var ("tell_to_colour") :
 446         (this_player()->defined_env_var ("tell_to_color") ? this_player()->query_env_var ("tell_to_color") : "");
 447
 448   user = sprintf("You %s %s: ", is_reply ? "reply to" : "tell", playerp(ob) ? capitalize(ob->query_real_name()) :
    ob->query_name());
 449
 450   printf("%s",ansi_colour(col));
 451
 452   if (this_player()->query_env_var("no_channels_wrap"))
 453     printf("%s%s\n",
 454              user,
 455              msg);
 456   else
 457     printf("%s%-=*s\n",
 458              user,
 459              this_player()->query_env_var("cols") ? clip(to_int(this_player()->query_env_var("cols")) -strlen(user),
    20, 200) : (78 -strlen(user)),
 460              msg);
 461
 462   printf("%s", ansi_colour("normal"));
 /obj/cmds/player/tell.c 462/493 :  463
 464   col = ob->query_env_var("tell_from_colour") || ob->query_env_var("tell_from_color") || "";
 465   user = sprintf("%s %s: ", capitalize(this_player()->query_real_name()), is_reply ? "replies" : "tells you");
 466
 467   efun::tell_object(ob, ansi_colour (col, ob));
 468
 469   if (ob->query_env_var("no_channels_wrap"))
 470     efun::tell_object(ob, sprintf("%s%s\n", user, msg));
 471   else if (ob->query_env_var("cols"))
 472     efun::tell_object(ob, sprintf("%s%-=*s\n", user, clip(to_int(ob->query_env_var("cols")) - strlen(user), 20, 200),
    msg));
 473   else
 474     efun::tell_object(ob, sprintf("%s%-=*s\n", user, 78 - strlen(user), msg));
 475
 476   efun::tell_object(ob, ansi_colour("normal", ob));
 477
 478   if (playerp(ob) && ob->query_away() && ob->query_away() != "")
 479     printf("Beware, %s is away %s.\n", capitalize(ob->query_real_name()), ob->query_away());
 480   if (this_player()->query_player_invis() || this_player()->query_wizard_invis() || this_player()->query_invis())
 481     printf("%sWarning: you are invisible, and might not be able to receive replies.%s\n", ANSI(bold red), ANSI
    (normal));
 482   return 1;
 483 }
 484
 /obj/cmds/player/tell.c 484/493 :  485 void clear_all_history ()
 486 {
 487   if (!adminp (this_player()))
 488     return;
 489   tell_history = ([ ]);
 490   save_object (SAVE_TELL);
 491 }
 492
 493
