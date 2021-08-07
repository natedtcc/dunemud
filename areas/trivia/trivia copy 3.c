// trivia.c
// based on deathknight's original trivia npc
// played a little differently now though, the question set is huge
// and the questions are randomly picked from the list, and the first
// person to MAX_POINTS points is the winner.
// Sauron

// Updated to use the OpenTBD API (https://opentdb.com) 
// Old questions file -> "/open/sauron/questions"
// Also added some stuff & cleaned up some of the nasties!
// Bebop 08/1/2021

inherit "/obj/player/monster";
#include "gen.h"
#include "ansi.h"

#define API        "/d/Atreid/domain/trivia/question_handler"
#define HEARER     "/players/arfang/listenner"  // Hearer
#define USED_LIST   "TRIVIA_ASKED"  // Old questions (already asked)
#define MAX_POINTS     200             // How many points do we play for?
#define WARMUP_TIME    60              // Time for players to join before start
#define ANSWER_TIME    15              // How long will players be able to answer?
#define BREAK_TIME     15              // Time between rounds 
#define OT_WARMUP_TIME 60              // Warmup for sudden death
#define MAX_ROUNDS     50              // Max number of rounds per game

mapping players,        // mapping in form of -> ([ name: score; correct guesses, last guest ])
        answered,       // Who has answered, and did they get it right?
        new_questions; // Mapping of new questions

string  *old_questions, // Questions we've already asked
        *waiting,       // People waiting for answers to be confirmed (accept, reject)
        already_asked,  // Questions that have already been asked
        *used_list,     // List of used questions
        *leader,        // Current game leader
        category,       // Category of the question
        curr_question,  // Question we're on right now
        letter_answer,  // The letter answer to the current question
        full_answer,    // The full correct answer
        *all_answers;    // All answers, wrong or right

object  GAMEMASTER;     // If a wiz is present, make him GM

int     round,          // What round are we in?
        automated = 1,  // Automated rounds (enabled)
        number_checked, // How man answers we've checked
        number_guessed, // How many answers yet to check
        in_round,       // Are we currently in a round?
        is_overtime,    // Are we in Sudden Death?
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
int do_answer(string who, string what);     // Check an answer
int do_accept();                 // Accept an answer as correct
int do_reject();                 // Reject an answer as incorrect
int got_right(string f);         // Got it right!
int got_wrong(string f);         // Oot it wrong :(
int do_add(string arg);          // Add points to someone's score
int do_subtract(string arg);     // Subtract points from someone's score
int quit_trivia();               // End the game suddenly
int is_automated();              // Is the game automated?
int register();                  // Register with the tell listener
int do_score();                  // Show the game score
int score_overtime();            // Check score of Sudden Death rounds

void do_question();              // (GM command) Show the question, and the time left to answer
void setup_question();           // Grab a question and parse it into question and answer
void display_question();         // (Game generated) Show the question, and the time you have to answer
void startup();                  // Initialize trivia, and set everything up
void end_round();                // End a current round
void set_automated(int state);   // Enable automation
void start_round_automation();   // Start the round automation sequence
void start_game_automation();    // Start the game automation sequence

void reset(int arg)
{
  GAMEMASTER = this_player();
  ::reset(arg);
  if (!arg) {
    
    set_level(100);
    set_prop("no_fight", 1);
    set_prop("no_voice",1);
    set_name("TriviaBot");
    set_alias(({"trivia_npc", "triviabot", "trivia"}));
    set_short("$BOLD$REDTriviaBot$OFF v2 (nasty)");
    set_long("\
This is a large, shiny robotic entity. He handles all the\n\
complexities of Trivia with ease! If you need help, type\n\
'tell bot /help' or 'help trivia' for more information.\n");

    enable_commands();
    players = ([ ]);
    answered = ([ ]);
    waiting = ({ });
    new_questions = ([]);
    old_questions = ({ });
  }
    register();
}

// Validate GAMEMASTER
int check_gm()
{
  if(!GAMEMASTER)
  {
    destruct(this_object());
    return 1;
  }

  if(!GAMEMASTER->query_wizard())
  {
    tell_object(GAMEMASTER, "Only wizards may be the game master!\n");
    destruct(this_object());
    return 1;
  }
}

// Start the trivia game, let everyone know we're here
void startup()
{
  tell_room(environment(this_object()), parse_ansi("$BOLD$GREENTrivia Started!\n"+
    "$WHITEGame Master: $YELLOW"+GAMEMASTER->query_name()+"$WHITE!$OFF\n"+
      "$CYANType 'tell trivia /help' for more information, or 'help trivia' for rules.$OFF\n"));
  tell_object(GAMEMASTER, parse_ansi(
      "$BOLD$YELLOW"+GAMEMASTER->query_name()+"$OFF$BOLD, you're in control of "
        +"$GREENTrivia$OFF$BOLD! Type '$REDtrivia$OFF$BOLD' for info.\n$OFF"));
  //HEARER->listen("A new game of Trivia is beginning!\n");
  round = 50;
  players = ([ ]);
  old_questions = ({ });

  start_game_automation();
}

// Give control to the GM
init()
{
  ::init();
  if(this_player() == GAMEMASTER)
  {
    // Added a message so wizards actually know the 'trivia' command even exists
    add_action("control","trivia");
  }
  else
  {
    tell_object(this_player(),
      "$CYANTriviaBot tells you: Greetings! Type 'tell trivia /help' for information!$OFF\n");
  }
}

int do_answer(string who, string what)
{
  object p;

  string temp_letter,
         temp_full;

  p = find_player(who);
  if(!answered) answered = ([ ]);

  if(!p)
    return 1;

  if(!present(p, E(GAMEMASTER)))
  {
    tell_object(p, "TriviaBot tells you: Whats that? Come closer...$OFF\n");
    return 1;
  }

  // added by Seeker 20010122
  if(!member(players, who))
  {
    tell_object(p, "TriviaBot tells you: You have to join first.\n");
    tell_object(p, "Tell trivia /join.\n");
    return 1;
  }

  if(!in_round)
  {
    tell_object(p, "TriviaBot tells you: We are not in a round right now!\n");
    return 1;
  }

  if(member(answered, who))
  {
    tell_object(p, "TriviaBot tells you: You've already answered this question!\n");
    return 1;
  }

  // added by Seeker 20010122
  if(member(waiting, who) != -1)
  {
    tell_object(p, "TriviaBot tells you: Your answer is already being processed.\n");
    return 1;
  }

  what = lower_case(what);
  temp_letter = lower_case(letter_answer);
  temp_full = lower_case(full_answer);

  if(what == temp_letter || what == temp_full)
  {
    got_right(who);
    return 1;
  }
  else
  {
    got_wrong(who);
    return 1;
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

  if(!present(ob, E(GAMEMASTER)))
  {
    tell_object(ob, "TriviaBot tells you: You must come to me to play!\n");
    return 1;
  }

  if(game_over)
  {
    tell_object(ob, "TriviaBot tells you: The game is already over.\n");
    return 1;
  }

  if(member(players, who))
  {
    tell_object(ob, "TriviaBot tells you: You are already playing!\n");
    return 1;
  }

  players += ([ who: 0; 0; 0,]);
  tell_room(environment(this_object()), parse_ansi(
    "TriviaBot announces: $CYAN"+capitalize(ob->QN)+"$OFF has joined Trivia!\n"));
  tell_object(ob, 
  "TriviaBot tells you: type 'tell trivia /help' for more information.$OFF\n");
  return 1;
}

int remove_player(string who)
{
  object ob;

  if(!players){
    players = ([ ]);
    write("TriviaBot tells you: You weren't playing!$OFF\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("TriviaBot tells you: You weren't playing!\n");
    return 1;
  }

  ob = find_player(who);

  if(!ob)
  {
    write("TriviaBot mutters: Not a player.\n");
    return 1;
  }

  if(game_over)
  {
    tell_object(ob, "TriviaBot tells you: The game is already over.\n");
    return 1;
  }

  m_delete(players, who);
  tell_object(ob, "TriviaBot tells you: Ok, goodbye!\n");
  tell_room(E(this_object()), parse_ansi(
    "TriviaBot announces: $RED"+capitalize(ob->QN)+"$WHITE has quit Trivia!\n"));
}

int do_help(string who)
{
  object ob;

  ob = find_player(who);
  if(!ob)
  {
    write("TriviaBot mutters: Not a player.\n");
    return 1;
  }

  tell_object(ob, "TriviaBot tells you: Lorem Ipsum'.\n"+
              "TriviaBot tells you: Lorem Ipsum.\n"+
              "TriviaBot tells you: Lorem Ipsum.\n"+
              "'tell trivia <answer>.\n");
  return 1;
}

int control(string str)
{
  if(!str){

    // Return a list of trivia commands so the GM
    // actually knows they exist - Bebop 07/29/21
    return notify_fail(
      "\nTrivia Commands:\n\n"
      +"'trivia round' - Start a round (generates a question and starts the timer)\n"
      +"'trivia automate <on/off> - Automate rounds, with "+BREAK_TIME+" seconds between rounds.\n"
      +"'trivia score' - Check the scoreboard.\n"
      +"'trivia accept <player>' - Accept a player's answer to a question.\n"
      +"'trivia reject <player> - Reject a player's answer.'\n"
      +"'trivia add <player> <amnt>' - Give a player points.\n"
      +"'trivia subtract <player> <amnt> - Take points away from a player.'\n"
      +"'trivia question' - Repeat the question to the room.\n"
      +"'trivia end' - End the game immediately.\n\n");
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
      write("TriviaBot tells you: Command not available.\n");
      break;
  }
  return 1;
}

int do_round()
{
  //if (!players){return notify_fail("Cannot start a round without players!\n");}
  int max_rec;

  max_rec = 50;

  if(!old_questions)
    old_questions = ({ });

  if(in_round)
  {
    write("TriviaBot tells you: We are already in a round!\n");
    return 1;
  }

  if(game_over)
  {
    write("TriviaBot tells you: The game is already over.\n");
    return 1;
  }

  
  in_round = 1;

  setup_question();

  // added by Seeker
  waiting = ({});

  setup_question();

  number_checked = number_guessed = 0;
//  log_file(USED_LIST, curr_question+"\n");

  display_question();
  call_out("end_round", ANSWER_TIME);
  return 1;
}

void setup_question()
{
  all_answers = ({});
  category = new_questions[round-1, 0];
  curr_question = new_questions[round-1, 1];
  letter_answer = new_questions[round-1, 2];
  full_answer = new_questions[round-1, 3];
  all_answers = new_questions[round-1, 4];
}

void display_question()
{
  string output;
  if (is_overtime) 
    say(parse_ansi("\n$BOLD$REDS U D D E N  D E A T H !$OFF\n"));
  

  else{ 
  say(parse_ansi(
    "\n$UNDERL$BOLDQuestion for round:$OFF$BOLD$YELLOW "
    +round+"/"+MAX_ROUNDS+"$OFF$BOLD\n"));
  }

  output = parse_ansi(sprintf("$BOLD%'-'-79s\n\n", "-"));
  output += parse_ansi(sprintf(
    "$BOLD$UNDERLCategory:$OFF$BOLD$BLUE  "+category
    +"\n\n$WHITE$UNDERLThe question is:$OFF"
    +"$BOLD$GREEN\n\n%s$WHITE\n\n\n",curr_question));
  output += parse_ansi(sprintf("$UNDERLPossible answers:$OFF$BOLD\n\n"));

  // Show the answers
  if (sizeof(all_answers)==2){
    output += parse_ansi(
      "$WHITEA) $CYAN" + all_answers[0] 
      + "  $WHITEor  B)$RED "+ all_answers[1]+"$WHITE\n\n");
  }
  
  else{
    for (int i=0 ;i<sizeof(all_answers);i++){
      switch(i) {
        case 0 : output += parse_ansi("$WHITEA) $CYAN" + all_answers[i]+"\n"); break;
        case 1 : output += parse_ansi("$WHITEB) $CYAN" + all_answers[i]+"\n"); break;
        case 2 : output += parse_ansi("$WHITEC) $CYAN" + all_answers[i]+"\n"); break;
        case 3 : output += parse_ansi("$WHITED) $CYAN" + all_answers[i]+"\n\n"); break;
      }
    }
  }

  output += parse_ansi(sprintf(
    "$BOLD$WHITEYou have $GREEN%d$WHITE seconds to answer.\n", ANSWER_TIME));
  output += parse_ansi(sprintf("%'-'-79s$OFF\n", "-"));

  say(output);
}

int do_score()
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
    write("TriviaBot tells you: The game is already over.\n");
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
    names = sort_array(names, "by_high_score", this_object());
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
  string player;

  if(!sizeof(waiting) || waiting[0] == 0)
  {
    write("TriviaBot tells you: You have already checked everyone!\n");
    number_checked = number_guessed;
    return 1;
  }

  player = waiting[0];
  write("TriviaBot tells you: You accept $GREEN"+capitalize(player)+"$OFF's answer.\n");
  got_right(player);
  waiting -= ({ player });
  number_checked++;
  if(number_checked == number_guessed && in_round == 0)
    award_points();
  return 1;
}

int do_reject()
{
  string player;

  if(!sizeof(waiting) || waiting[0] == 0)
  {
    write("TriviaBot tells you: You've already checked everyone!\n");
    number_checked = number_guessed;
    return 1;
  }

  player = waiting[0];
  write("You reject $YELLOW"+capitalize(player)+"$OFF's answer.\n");

  got_wrong(player);
  waiting -= ({ player });
  number_checked++;
  if(number_checked == number_guessed && in_round == 0)
    award_points();
  return 1;
}

int got_right(string who)
{
  say(parse_ansi(
    "$GREENTriviaBot says: "+capitalize(who)+" got it right!$OFF\n"));
  players[who, 1]++;
  answered += ([ who: 1, ]);

  players[who, 2] = 1;
  // Mark the player as having correctly answered
  return 1;
}

int got_wrong(string who)
{
  say(parse_ansi(
    "$CYANTriviaBot says: "+capitalize(who)+" got it wrong!$OFF\n")); 
  answered += ([ who: 0, ]);

  // Mark the player as having incorrectly answered
  players[who, 2] = 0;
  return 1;
}

// Give a player some points

int do_add(string arg)
{
  string who;
  int howmuch;

  if (!arg){
    return notify_fail("Syntax: 'trivia add <player> <amount>'\n");
  }

  if(sscanf(arg, "%s %d", who, howmuch) != 2)
  {
    write("TriviaBot tells you: Syntax: trivia add <player> <amt>\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("TriviaBot tells you: That person isn't playing!\n");
    return 1;
  }

  if(!howmuch || howmuch < 0)
  {
    write("TriviaBot tells you: Try trivia subtract "+who+" "+howmuch+"\n");
    return 1;
  }

  players[who, 0] += howmuch;
  say(GAMEMASTER->query_name()+" adds "+howmuch+" points to "+capitalize(who)+"'s score!\n");
  return 1;
}

// Remove points from a player

int do_subtract(string arg)
{
  string who;
  int howmuch;

    if (!arg){
    return notify_fail("Syntax: 'trivia add <player> <amount>'\n");
  }

  if(sscanf(arg, "%s %d", who, howmuch) != 2)
  {
    write("TriviaBot tells you: Syntax: trivia subtract <player> <amt>\n");
    return 1;
  }

  if(!member(players, who))
  {
    write("TriviaBot tells you: That person isn't playing!\n");
    return 1;
  }

  if(!howmuch || howmuch < 0)
  {
    write("TriviaBot tells you: Try trivia add "+who+" "+howmuch+"\n");
    return 1;
  }

  players[who, 0] -= howmuch;
  say(GAMEMASTER->query_name()+" subtracts "+howmuch+" points from "+capitalize(who)+"'s score!\n");
  return 1;
}


// Repeat the quesiton / time remaining
void do_question()
{
  say("The question is: "+curr_question+"\n");
  say("You have "+find_call_out("end_round")+" seconds left to answer.\n");
}

// Kill trivia
int quit_trivia()
{
  say(parse_ansi(
    "$BOLDThe Game Master has $REDended$WHITE this game of $GREENTrivia.$OFF\n"));
  say(parse_ansi(
    "$BOLD$REDTriviaBot$OFF emits a loud $BOLD$YELLOWbeep$OFF and self-destructs, "
    +"$UNDERL$BOLDE$REDX$WHITEP$REDL$WHITEO$REDD$WHITEI$REDN$WHITEG$OFF into bits!\n"));
  
  // Kill the API
  // call_other(API, "clean_up", 1);
  
  // Byeeeeee
  destruct(this_object());
  return 1;
}

void end_round()
{
  in_round = 0;

  say(parse_ansi("\n$BOLD$REDTIME'S UP!$OFF\n\n"));

  award_points();

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
  say(parse_ansi(sprintf(
    "$BOLD%'-'79s\n"
    +"$BOLDThe answer was:  $WHITE"
    +capitalize(letter_answer)+")"
    +"$CYAN "+full_answer+"$WHITE\n"
    +"$BOLD%'-'79s$OFF\n", "-", "-")));

  // Find out got it right
  for(i=0; i<total_players; i++)
  {
    if(member(answered, names[i]) && answered[names[i]] == 1)
    {
      winners += ({ names[i], });
    }
  }

  if(!sizeof(winners))
  {
    say(parse_ansi("$BOLDNo one got that one right!$OFF\n"));
    answered = ([ ]);
    
    if (is_overtime)
      score_overtime();
    
    else
      check_points();
    
    return 1;
  }

  // Calculate how much the winners get
  // total_awarded = 10 * total_players;
  // award_amount = total_awarded/sizeof(winners);
  award_amount = 10;

  // Award the winnings to the winners
  for(i=0; i<sizeof(winners); i++)
  {
    players[winners[i], 0] += award_amount;
  }

  // Announce the winnings
  say(parse_ansi("$BOLD$YELLOW"+award_amount+"$WHITE points awarded to "));
  for(i=0; i<sizeof(winners); i++)
  {
    if(i == sizeof(winners)-1)
      say(parse_ansi("$BOLD$CYAN"+capitalize(winners[i])+"$OFF$BOLD."));
    else
      say(parse_ansi("$BOLD$CYAN"+capitalize(winners[i])+", "));
  }
  say("\n");

  if (is_overtime)
    score_overtime();

  // Check to see if the game is over
  else check_points();

  answered = ([ ]);
  return 1;
}

/* Game will now end if a player (or players) points are
*  >= MAX_POINTS , OR if the current round > MAX_ROUNDS
*  in the latter case, the leader is declared the winner. */

int check_points()
{
  // if (sizeof(players) < 4){
  //   say(parse_ansi("$BOLDNot enough players to continue!$OFF\n"));
  //   set_automated("off");
  //   quit_trivia();
  //   }

  string *names,
         *winners;

  int     leader_score = -1;

  object     who;

  names = m_indices(players);
  winners = ({ });

  for(int i=0; i<sizeof(names); i++)
  {
    if (round==MAX_ROUNDS){
      if (leader_score<=players[names[i], 0]){
        leader_score = players[names[i], 0];
        winners += ({ names[i] });
      }
      else {
        who = find_player(names[i]);
        tell_object(who, parse_ansi(
          "$BOLDSorry, you $REDlost$WHITE. Better luck next time!\n"));
      }
    }

    if(players[names[i], 0] >= MAX_POINTS)
    {
      winners += ({ names[i], });
      game_over++;
    }
  }

  // If MAX_POINTS or MAX_ROUNDS are met, end game
  if(game_over || round==MAX_ROUNDS){
    game_over++;
    set_automated(0);
    end_game(winners);
  }
  
  else if (is_automated() && !game_over){
    round++;
    start_round_automation();
  }
  

  return 1;
}

int end_game(string *winners)
{
  int i,
      highest;

  string final_winner,
         *tie_list,
         output,
         tie_winners = "";

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

//Indexing on illegal val?

  // Did we have a tie?
  if(final_winner == "tie")
  {
    say(parse_ansi(
      "$BOLDWE HAVE A $CYANTIE$WHITE!$OFF\n"));
    players = ([]);
    // m_delete(players, who);
    for(i=0; i<sizeof(tie_list); i++)
    {
      players += ([ tie_list[i]: 0; 0; 0,]);
      tie_list[i] = capitalize(tie_list[i]);
    }
    
    tie_winners = _implode(tie_list, ", ", " and ");
    shout(parse_ansi(
     "\n$BOLD$REDS U D D E N  D E A T H$WHITE\n"
     +"$BOLD$CYAN"+tie_winners+" $WHITEhave tied in $GREENTrivia!!$OFF\n"));
    say(parse_ansi(
      "\n$BOLD$REDSUDDEN DEATH$WHITE WILL BEGIN IN $GREEN"
      +OT_WARMUP_TIME/60+"$WHITE MINUTE!$OFF\n"
      +"In sudden death, you must answer questions correctly to stay in the\n"
      +"game. If a player answers incorrectly, and their opponent answers\n"
      +"correctly, the player will lose. This rule also applies to non-answers,\n"
      +"as not answering a question is equal to answering incorrectly. However,\n"
      +"if both players fail to answer, the round is pushed.\n\n"
      +"$BOLD GOOD LUCK!$OFF\n"));
      
      // Re-enable automation, enable overtime, reset the round, update questions
      set_automated(1);
      is_overtime = 1;
      round = 1;
      load_object(API)->update_trivia_data();
      call_out("alert_overtime", (OT_WARMUP_TIME/3*2));
      return 1;
  }

  else
  {
    say(parse_ansi("$BOLDThe winner is: $CYAN"+capitalize(final_winner)+"$WHITE!\n"));
    shout("The Trivia game has ended. "+capitalize(final_winner)+" is the champion!\n");
  }

  say("Congratulations! And thank you all for playing Dune Trivia!\n");
  game_over = 0;
  do_score();
  game_over = 1;

  // Kill the API
  //call_other(API, "clean_up", 1);
  destruct(this_object());
  return 1;
}

// Overtime

void alert_overtime(){
  
  say(parse_ansi(
    "\n$BOLD$REDSUDDEN DEATH$OFF WILL BEGIN IN $GREEN"
    +OT_WARMUP_TIME/3+"$WHITE SECONDS!$OFF\n\n"));

  // Grab a fresh set of questions from OpenTriviaDB
  new_questions = load_object(API)->query_trivia_data();
  tell_object(GAMEMASTER,"TOTAL QUESITONS: " + sizeof(new_questions) + "\n");
  
  if(sizeof(new_questions) < 1){
    tell_room(environment(this_object()),
      "Question list empty! API Error! Please MudMail Bebop!\n");
    return;
  }  

  call_out("warn_overtime", OT_WARMUP_TIME/4);
}


void warn_overtime(){
  say(parse_ansi(
    "\n$BOLD$REDSUDDEN DEATH$OFF WILL BEGIN IN $GREEN"
    +OT_WARMUP_TIME/12+"$WHITE SECONDS!$OFF\n\n"));
  
  call_out("do_round", OT_WARMUP_TIME/12);

}

int score_overtime(){
  string* names;

  names = m_indices(players);

  // If someone answered wrong, remove them from the playerlist
  for (int i = 0; i<sizeof(names); i++){
    say(names[i]);
    if (players[names[i], 2] == 0){
      m_delete(players, names[i]);
    }
  }

  if (sizeof(players)==1){
    names = m_indices(players);
    say(parse_ansi("$BOLD$CYAN"+names[0]+"$WHITE HAS WON IN SUDDEN DEATH!\n"));
    return 1;
  }

  // If there are no players left, everyone got it wrong, so its a push
  else if (sizeof(players)==0){
    for (int i=0;i<sizeof(names);i++){
      players += ([names[i]: 0; 0; 0,]);
    }
  }
  
  round++;
  start_round_automation();
  return 1;
}


// Automate the game.
void start_game_automation(){
  if (is_automated()){
    // Make a call to the API
    load_object(API)->update_trivia_data();
    shout(parse_ansi("\n\n$BOLDTrivia is starting in $GREEN"
      +WARMUP_TIME+"$OFF$BOLD seconds!$OFF\n"
      +"$BOLDHead to the Trivia room (up from Caladan AP) and type:\n"
      +"'$CYANtell trivia /join$WHITE' "
      +"to join!\n\n$OFF"));

    call_out("warn_game_automation", to_int(WARMUP_TIME*0.834));
  }
}


void warn_game_automation(){
  if (is_automated()){
    shout(parse_ansi("\n\n$BOLDTrivia is starting in $GREEN"
      +WARMUP_TIME/6+"$OFF$BOLD seconds!$OFF\n"
      +"$BOLDHead to the Trivia room (up from Caladan AP) and type:\n"
      +"'$CYANtell trivia /join$WHITE' "
      +"to join!\n\n$OFF"));
      
    // Get the questions from the API
    new_questions = load_object(API)->query_trivia_data();

    // If no questions are loaded, kill trivia
    if(sizeof(new_questions) < 1){
      tell_room(environment(this_object()),
        "Question list empty! API Error! Please Mudmail Bebop!\n");
      return;
  }
    call_out("finish_game_automation", WARMUP_TIME/6);
  }
}

//&& sizeof(players) > 2)

void finish_game_automation(){
  if(is_automated()) {
    do_round();
  }

  else {
    tell_room(environment(this_object()), 
      parse_ansi("$BOLDNot enough players joined!\n"
      +"$REDAborting$WHITE game!$OFF\n"));
    quit_trivia();
  }
}


// Automate the rounds.

void start_round_automation(){
  if (is_automated()){
    say(parse_ansi("\n\n$BOLDRound$RED "+round+"$WHITE starting in $GREEN"
      +BREAK_TIME+"$OFF$BOLD seconds!$OFF\n"));
    call_out("warn_round_automation", to_int(BREAK_TIME*0.834));
  }
}

void warn_round_automation(){
  if (is_automated()){
    say(parse_ansi("\n$BOLDRound$RED "+round+"$WHITE starting in $GREEN"
      +BREAK_TIME/6+"$OFF$BOLD seconds!$OFF\n"));
    call_out("finish_round_automation", BREAK_TIME/6);
  }
}

void finish_round_automation(){
  if(is_automated()) {
    do_round();
  }
}

void set_automated(int state){
  automated = state;
}


int is_automated(){
  return automated;
}


int register() { 
  if ("/obj/cmds/player/tell"->add_tell_handler("triviabot",
    this_object())){ 
    return "/obj/cmds/player/tell"->add_tell_handler("trivia",
      this_object()); 
  }
  else return notify_fail(
    "Unable to set tell handler. Please consult a wizard.\n");
}

int set_rounds(int i){
  round = i;
  return round;
}

mapping get_players(){
  return players;
}

string did_answer(string who){
  return players[who, 2];
}

// Process input

int hear_tell(string user, string mesg, int reply){ 
  string      who = lower_case(user->query_name());
  map(filter_objects(users(), "id", getuid()), #'tell_object,
    sprintf("%#Q::hear_tell(%#Q, %#Q, %#Q);\n", this_object(), user, mesg, reply));
    
    switch (mesg)
    {
      case "/help": do_help(who); break;
      case "/join": add_player(who); break;
      case "/quit": remove_player(who); break;
      case "/scores": do_score(); break;
      default: do_answer(who, mesg); break;
    }
    if(find_player(who)) find_player(who)->add_sp(1);

  return 1;
  }

  


