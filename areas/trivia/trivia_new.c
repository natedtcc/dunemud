// trivia.c
// based on deathknight's original trivia npc
// played a little differently now though, the question set is huge
// and the questions are randomly picked from the list, and the first
// person to MAX_POINTS points is the winner.
// Sauron

inherit "/obj/player/monster";
#include "gen.h"
#include "ansi.h"

#define QUESTIONS "/open/sauron/questions" // Where the questions are at
#define USED_LIST "TRIVIA_ASKED"  // Old questions (already asked)
#define MAX_POINTS 500        // How many points do we play for?
#define TIME 24               // How long do the rounds last?

mapping players,    // mapping in form of -> ([ name: score; correct guesses, ])
        answered;   // Who has answered, and did they get it right?

string  *new_questions, // Array of new questions
        *old_questions, // Questions we've already asked
        *waiting,       // People waiting for answers to be confirmed (accept, reject)
        question_list,  // Parsed and put into the new_questions array when we
        already_asked,  // Questions that have already been asked
        *used_list,     // List of used questions
        curr_question,  // Question we're on right now
        alt_answer,     // Alternate answer to current question
        answer;         // The answer to the current question

object GM;          // Big brother

int round,          // What round are we in?
    number_checked, // How man answers we've checked
    number_guessed, // How many answers yet to check
    in_round,       // Are we currently in a round?
    game_over;      // Is the game over?

// Function Prototyping
int check_gm();                  // Make sure we have a valid game master
int add_player(string who);      // Add a player to the game
int remove_player(string who);   // Remove a player from the game
int do_help(string who);         // Give a player help
int control(string str);         // Control commands for the GM
int do_round();                  // Begin a trivia round
int award_points();              // Give points to the winners of the round
int check_points();              // Check to see if the game is over
int end_game(string *winners);   // End the game, congratulate winners
do_score();                      // Show the game score
int do_accept();                 // Accept an answer as correct
int do_reject();                 // Reject an answer as incorrect
int got_right(string f);         // Got it right!
int got_wrong(string f);         // Ooops, fucked that one up
int do_add(string arg);          // Add points to someone's score
int do_subtract(string arg);     // Subtract points from someone's score
int quit_trivia();               // End the game suddenly

void do_question();              // (GM command) Show the question, and the time left to answer
void setup_question();           // Grab a question and parse it into question and answer
void display_question();         // (Game generated) Show the question, and the time you have to answer
void startup();                  // Initialize trivia, and set everything up
void end_round();                // End a current round

reset(arg)
{
  GM = TP;
  ::reset(arg);
  if(arg)return;

  if(check_gm())
   return;

  set_level(100);
  set_prop("no_fight", 1);
  set_prop("no_voice",1);
  set_name("pursuit");
  set_alias("trivia");
  set_short("Mentat Pursuit, the Trivia Extraordinaire");
  set_long("\
This is the Dune Mud Trivia Extraordinaire.\n\
Type 'tell pursuit help' for more information.\n");

  enable_commands();
  players = ([ ]);
  answered = ([ ]);
  waiting = ({ });
  new_questions = ({ });
  old_questions = ({ });

  startup();
}

// Validate GM
int check_gm()
{
  if(!GM)
  {
    destruct(TO);
    return 1;
  }

  if(!GM->query_wizard())
  {
    tell_object(GM, "Only wizards may be the game master!\n");
    destruct(TO);
    return 1;
  }
}

// Initialize the trivia game, let everyone know we're here
void startup()
{
  tell_object(GM, RED+"Trivia started!\n"+OFF);
  move_object(TO, environment(GM));
  tell_room(E(TO), GREEN+"Trivia Pursuit Started!\n"+
           "Gamemaster: "+OFF+GM->QN+"!\n"+
           CYAN+"Type 'tell pursuit /help' for more information!\n"+OFF);
  shout(RED+"Trivia Pursuit Initiated!\n"+OFF);
  round = 0;
  players = ([ ]);

  question_list = read_file(QUESTIONS);          // Read the whole file
  if(question_list)
    new_questions = explode(question_list, "\n");  // Put it into this array by line
  else
  {
    tell_object(GM, "Question list empty. Aborting.\n");
  }
  old_questions = ({ });                         // Initialize old questions

  already_asked = read_file("/log/"+USED_LIST);
  if(already_asked)
    used_list = explode(already_asked, "\n");
  old_questions = used_list;
//  if(!old_questions)
    old_questions = ({ });
}

// Give control to the GM
init()
{
  ::init();
  if(TP == GM)
  {
    // Added a message so wizards actually know the 'trivia' command even exists - Bebop 2021
    add_action("control","trivia");
    tell_object(GM, GREEN+"Mentat Pursuit tells you: You're in control of trivia! Type 'trivia' for info.\n");
  }
  else
  {
    tell_object(TP, GREEN+"Mentat Pursuit tells you: Welcome! Type 'tell pursuit /help' for information!\n"+OFF);
  }
}

// Process input
catch_tell(string str)
{
  string who,
         what;

  if(sscanf(str, "%s tells you: %s\n", who, what) == 2)
  {
    who = lower_case(who);
    what = lower_case(what);
    switch(what)
    {
      case "/help": do_help(who); break;
      case "/join": add_player(who); break;
      case "/quit": remove_player(who); break;
      default: do_answer(who, what); break;
    }
    if(find_player(who)) find_player(who)->add_sp(1);
  }
  return 1;
}

int do_answer(string who, string what)
{
  object p;

  p = find_player(who);
  if(!answered) answered = ([ ]);

  if(!p)
    return 1;

  if(!present(p, E(TO)))
  {
    tell_object(p, "Pursuit tells you: Whats that? Come closer...\n");
    return 1;
  }

  // added by Seeker 20010122
  if(!member(players, who))
  {
    tell_object(p, "Pursuit tells you: You have to join first.\n");
    tell_object(p, "Tell pursuit /join.\n");
    return 1;
  }

  if(!in_round)
  {
    tell_object(p, "Pursuit tells you: We are not in a round right now!\n");
    return 1;
  }

  if(member(answered, who))
  {
    tell_object(p, "Pursuit tells you: You've already answered this question!\n");
    return 1;
  }

  // added by Seeker 20010122
  if(member(waiting, who) != -1)
  {
    tell_object(p, "Pursuit tells you: Your answer is already being processed.\n");
    return 1;
  }

  what = lower_case(what);
  answer = lower_case(answer);
  alt_answer = lower_case(alt_answer);

  if(what == answer || what == alt_answer)
  {
    got_right(who);
    return 1;
  }
  else
  {
    waiting += ({ who, });
    tell_object(GM, "\nPursuit tells you: The answer is "+CYAN+capitalize(answer)+OFF+" OR "+CYAN+capitalize(alt_answer)+OFF+"\n");
    tell_object(GM, "Pursuit tells you: "+GREEN+capitalize(who)+OFF+" guessed "+YELLOW+capitalize(what)+OFF+"\n\n");
    number_guessed++;
  }
}


int add_player(string who)
{
  object ob;

  if(!players) { players = ([ ]); }

  if(!who)
  {
    write("No argument specified.\n");
    return 1;
  }

  ob = find_player(who);

  if(!ob)
  {
    write("Could not find target.\n");
    return 1;
  }

  if(!present(ob, E(TO)))
  {
    tell_object(ob, "Pursuit tells you: You must come to me to play!\n");
    return 1;
  }

  if(game_over)
  {
    tell_object(ob, "Pursuit tells you: The game is already over.\n");
    return 1;
  }

  if(member(players, who))
  {
    tell_object(ob, "Pursuit tells you: You are already playing!\n");
    return 1;
  }

  players += ([ who: 0; 0, ]);
  tell_room(E(TO), "Pursuit announces: "+CYAN+capitalize(ob->QN)+OFF+" has joined Trivia Pursuit!\n");
  tell_object(ob, "Pursuit tells you: type 'tell pursuit /help' for more information.\n");
  return 1;
}

int remove_player(string who)
{
  object ob;

  if(!players)
  {
    players = ([ ]);
    write("Pursuit tells you: You weren't playing!\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("Pursuit tells you: You weren't playing!\n");
    return 1;
  }

  ob = find_player(who);

  if(!ob)
  {
    write("Pursuit mutters: Not a player.\n");
    return 1;
  }

  if(game_over)
  {
    tell_object(ob, "Pursuit tells you: The game is already over.\n");
    return 1;
  }

  m_delete(players, who);
  tell_object(ob, "Pursuit tells you: Ok, goodbye!\n");
  tell_room(E(TO), "Pursuit announces: "+RED+capitalize(ob->QN)+OFF+" has QUIT Trivia Pursuit!\n");
}

int do_help(string who)
{
  object ob;

  ob = find_player(who);
  if(!ob)
  {
    write("Pursuit mutters: Not a player.\n");
    return 1;
  }

  tell_object(ob, "Pursuit tells you: Ok, this is easy. To join, type 'tell pursuit /join'.\n"+
              "Pursuit tells you: To quit, type 'tell pursuit /quit'.\n"+
              "Pursuit tells you: To answer a question, after it is asked, just type\n"+
              "'tell pursuit <answer>. Pretty simple, really.\n");
  return 1;
}

int control(string str)
{
  if(str == ""){
    return_notify_fail(
      "Trivia Commands:\n\n"
      +"'trivia round'\n"
      +"'trivia score'\n"
      +"'trivia accept'\n"
      +"'trivia reject'\n"
      +"'trivia add'\n"
      +"'trivia subtract'\n"
      +"'trivia question'\n"
      +"'trivia end' - End the game immediately.\n\n")
  }     

  string cmd,
         arg;

  int double;

  if(sscanf(str, "%s %s", cmd, arg) == 2)
    double = 1;
  if(!double)
    sscanf(str, "%s", cmd);

  switch(cmd)
  {
    case "round": do_round(); break;
    case "score": do_score(); break;
    case "accept": do_accept(); break;
    case "reject": do_reject(); break;
    case "add": do_add(arg); break;
    case "subtract": do_subtract(arg); break;
    case "question": do_question(); break;
    case "end": quit_trivia(); break;
    default:
      write("Pursuit tells you: Command not available.\n");
      break;
  }
  return 1;
}

int do_round()
{
  int max_rec;

  max_rec = 50;

  if(!old_questions)
    old_questions = ({ });

  if(in_round)
  {
    write("Pursuit tells you: We are already in a round!\n");
    return 1;
  }

  if(game_over)
  {
    write("Pursuit tells you: The game is already over.\n");
    return 1;
  }

  round++;
  in_round = 1;

  setup_question();

  // added by Seeker
  waiting = ({});

  while(member(old_questions, curr_question) != -1 && max_rec > 0)
  {
    setup_question();
    max_rec--;
  }
  if(max_rec == 0)
  {
    say("ERROR! QUESTION COULD NOT BE GENERATED. ABORTING.\n");
    in_round = 0;
    return 1;
  }

  number_checked = number_guessed = 0;
  old_questions += ({ curr_question, });
  log_file(USED_LIST, curr_question+"\n");

  display_question();
  call_out("end_round", TIME);
  return 1;
}

void setup_question()
{
  string *tmp_question;

  tmp_question = explode(new_questions[random(sizeof(new_questions))], "#");

  curr_question = tmp_question[0];
  answer = tmp_question[1];
  alt_answer = tmp_question[1]; // Some dude fucked up questions no alt answers
}

void display_question()
{
  string output;

  say("Question for round "+round+".\n");
  output = sprintf("%'-'-79s\n\n", "-");
output += sprintf("The question is: %s\n",curr_question);
  output += sprintf("\n\n");
  output += sprintf("You have "+GREEN+"%d"+OFF+" seconds to answer.\n", TIME);
  output += sprintf("%'-'-79s\n", "-");

  say(output);
}

do_score()
{
  string *names,
         *left,
         *right,
         output;

  mapping top,
          bottom;

  int count,
      howmany,
      i;

  names = m_indices(players);
  output = "";

  top = ([ ]);
  bottom = ([ ]);

  if(game_over)
  {
    write("Pursuit tells you: The game is already over.\n");
    return 1;
  }

  output += sprintf("Trivia Scores for round %d\n", round);
  output += sprintf("%'-'60s\n", "-");
  say(output);
  output = "";

  left = ({ });
  right = ({ });

  if(sizeof(names) > 1)
  {
    names = sort_array(names, "by_high_score", TO);
    left = names[0..sizeof(names)/2];
    for(i=0;i<sizeof(names);i++)
    {
      if(member(left, names[i]) == -1)
        right += ({ names[i], });
    }

    for(i=0; i < sizeof(names)/2+1; i++)
    {
      if(sizeof(left) >= i)
      {
        top += ([ left[i]: players[left[i], 0]; players[left[i], 1], ]);
      }
      if(sizeof(right)-1 >= i)
      {
        bottom += ([ right[i]: players[right[i], 0]; players[right[i], 1], ]);
      }
    }
  }
  else
  {
     say(sprintf("%-3s%-15s%-4d(%d%%) \n", "1.", capitalize(names[0]), players[names[0], 0],
         players[names[0], 1]*100/(round > 0 ? round : 1)));
     say(sprintf("%'-'60s\n", "-"));
     return 1;
  }

  howmany = sizeof(m_indices(top));
  left = m_indices(top);
  left = sort_array(left, "by_high_score");
  right = m_indices(bottom);
  right = sort_array(right, "by_high_score");

  for(i=0; i<howmany; i++)
  {
      output = "("+to_string(top[left[i], 1]*100/(round > 1 ? round : 1))+"\%)";
      say(sprintf("%-3s%-15s%-4d%-8s", to_string(i+1)+".", capitalize(left[i]),
          top[left[i], 0], output));

      if(sizeof(right) > i)
      {
        output = "("+to_string(top[right[i], 1]*100/(round > 1 ? round : 1))+"\%)";
        say(sprintf("%-3s%-15s%-4d%-8s", to_string(sizeof(left)+1+i)+".",
            capitalize(right[i]), bottom[right[i], 0],
            "("+to_string(bottom[right[i], 1]*100/(round > 1 ? round : 1))+"%)"));
      }
      say("\n");
  }
  say(sprintf("%'-'60s\n", "-"));

  return 1;
}

status by_high_score(string s, string f)
{
  return players[s, 0] < players[f, 0];
}

int do_accept()
{
  string fucker;

  if(!sizeof(waiting) || waiting[0] == 0)
  {
    write("Pursuit tells you: You have already checked everyone!\n");
    number_checked = number_guessed;
    return 1;
  }

  fucker = waiting[0];
  write("Pursuit tells you: You accept "+GREEN+capitalize(fucker)+OFF+"'s answer.\n");
  got_right(fucker);
  waiting -= ({ fucker });
  number_checked++;
  if(number_checked == number_guessed && in_round == 0)
    award_points();
  return 1;
}

int do_reject()
{
  string fucker;

  if(!sizeof(waiting) || waiting[0] == 0)
  {
    write("Pursuit tells you: You've already checked everyone!\n");
    number_checked = number_guessed;
    return 1;
  }

  fucker = waiting[0];
  write("You reject "+YELLOW+capitalize(fucker)+OFF+"'s answer.\n");

  got_wrong(fucker);
  waiting -= ({ fucker });
  number_checked++;
  if(number_checked == number_guessed && in_round == 0)
    award_points();
  return 1;
}

int got_right(string f)
{
  say(GREEN+"Pursuit says: "+capitalize(f)+" got it right!\n"+OFF);
  players[f, 1]++;
  answered += ([ f: 1, ]);
  return 1;
}

int got_wrong(string f)
{
  say(CYAN+"Pursuit says: "+capitalize(f)+" got it wrong!\n"+OFF);
  answered += ([ f: 0, ]);
  return 1;
}

int do_add(string arg)
{
  string who;
  int howmuch;

  if(sscanf(arg, "%s %d", who, howmuch) != 2)
  {
    write("Pursuit tells you: Syntax: trivia add <player> <amt>\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("Pursuit tells you: That person isn't playing!\n");
    return 1;
  }

  if(!howmuch || howmuch < 0)
  {
    write("Pursuit tells you: Try trivia subtract "+who+" "+howmuch+"\n");
    return 1;
  }

  players[who, 0] += howmuch;
  say(GM->query_name()+" adds "+howmuch+" points to "+capitalize(who)+"'s score!\n");
  return 1;
}

int do_subtract(string arg)
{
  string who;
  int howmuch;

  if(sscanf(arg, "%s %d", who, howmuch) != 2)
  {
    write("Pursuit tells you: Syntax: trivia subtract <player> <amt>\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("Pursuit tells you: That person isn't playing!\n");
    return 1;
  }

  if(!howmuch || howmuch < 0)
  {
    write("Pursuit tells you: Try trivia add "+who+" "+howmuch+"\n");
    return 1;
  }

  players[who, 0] -= howmuch;
  say(GM->query_name()+" subtracts "+howmuch+" points from "+capitalize(who)+"'s score!\n");
  return 1;
}

void do_question()
{
  say("The question is: "+curr_question+"\n");
  say("You have "+find_call_out("end_round")+" seconds left to answer.\n");
}

int quit_trivia()
{
  say("Trivia ENDED by the Game Master!\n");
  say("Mentat Pursuit takes off, crying.\n");
  destruct(TO);
  return 1;
}

void end_round()
{
  in_round = 0;

  say(RED+BLINK+"\nTIME'S UP!\n\n"+OFF);

  if(number_checked == number_guessed)
  {
    award_points();
  }
}

int award_points()
{
  int total_players,
      total_awarded,
      award_amount,
      i;

  string *names,              // All players names
         *winners;            // Just the winners names

  names = m_indices(players);
  total_players = sizeof(names);
  if(!answered) answered = ([ ]);
  winners = ({ });

  // Show everyone the answer
  say(sprintf("%'-'79s\n", "-"));
  say("The answer was: "+answer+"\n");
  say(sprintf("%'-'79s\n", "-"));

  // Find out who won
  for(i=0; i<total_players; i++)
  {
    if(member(answered, names[i]) && answered[names[i]] == 1)
    {
      winners += ({ names[i], });
    }
  }

  if(!sizeof(winners))
  {
    say("Nobody got it right! You guys suck!\n");
    answered = ([ ]);
    check_points();
    return 1;
  }

  // Calculate how much the winners get
  total_awarded = 10 * total_players;
  award_amount = total_awarded/sizeof(winners);

  // Award the winnings to the winners
  for(i=0; i<sizeof(winners); i++)
  {
    players[winners[i], 0] += award_amount;
  }

  // Announce the winnings
  say(award_amount+" points awarded to ");
  for(i=0; i<sizeof(winners); i++)
  {
    if(i == sizeof(winners)-1)
      say(capitalize(winners[i])+". ");
    else
      say(capitalize(winners[i])+", ");
  }
  say("\n");

  // Check to see if the game is over
  check_points();

  answered = ([ ]);
  return 1;
}

int check_points()
{
  int i;

  string *names,
         *winners;

  names = m_indices(players);
  winners = ({ });

  for(i=0; i<sizeof(names); i++)
  {
    if(players[names[i], 0] >= MAX_POINTS)
    {
      winners += ({ names[i], });
      game_over++;
    }
  }

  if(game_over)
    end_game(winners);

  return 1;
}

int end_game(string *winners)
{
  int i,
      highest;

  string final_winner,
         *tie_list;

  say("We have a winner!\n");

  // If we have more than one winner, find who had more points
  if(sizeof(winners) > 1)
  {
    for(i=0;i<sizeof(winners);i++)
    {
      if(players[winners[i], 0] > highest)
      {
        highest = players[winners[i], 0];
        final_winner = winners[i];
      }
      else if(players[winners[i], 0] == highest)
      {
        tie_list = ({ final_winner, winners[i] });
        final_winner = "tie";
      }
    }
  }
  else
    final_winner = winners[0];

  // Did we have a tie?
  if(final_winner == "tie")
  {
    say("Err, actually, it looks like we had a tie!\n");
    say("The winners are: ");
    for(i=0; i<sizeof(tie_list); i++)
    {
      say(CYAN+BLINK+capitalize(tie_list[i])+OFF+", ");
    }
    say("\n");
  }

  else
  {
    say("The winner is: "+CYAN+BLINK+capitalize(final_winner)+OFF+"!\n");
  }

  say("Congratulations! And thank you all for playing Dune Trivia!\n");
  game_over = 0;
  do_score();
  game_over = 1;

  destruct(TO);
  return 1;
}
