/*
 * Trivia Question Generator
 * modified Dox's example to better serve trivia
 *   - Paradox@DUNE - 2021-07-04
 *   - Bebop@DUNE - 2021-08-01
 */
#pragma strict_types, save_types, rtt_checks, pedantic, no_shadow, no_clone
#define DEBUG_TARGET "bebop"
#define DEBUG_OFF
// #include <debug.h>

#include "/secure/obj/py_http/py_http.h"

inherit "/secure/obj/py_http/py_http_types";
inherit "/players/bebop/open/decode_html";

#define PY_HTTP_CLIENT "/secure/obj/py_http/py_http_client.c"
#define API_BASE_URL "https://opentdb.com/api.php"

// Clean up the handler
public int clean_up(int arg);

private mapping triviaData;

public void question_handler(struct HTTPResponse resp) {
  mixed data = json_parse(resp->body);    // API Response
  mapping questionData;                   // Current question data
  string letterAnswer;                    // Letter value of answer
  string* shuffledAnswers;                // All answers, shuffled
  string* formattedAnswers;               // All answers, formatted

  /* Build an mapping of trivia data for Trivia
  * 
  */

  if(sizeof(data["results"]) > 0) {
    triviaData = m_allocate(sizeof(data["results"]), 5);
    for (int i=0; i < sizeof(data["results"]); i++){
      questionData = data["results"][i];
      
      // Assign the category and question, decoded
      triviaData[i, 0] = decode(questionData["category"]);
      triviaData[i, 1] = decode(questionData["question"]);
      
      // For true / false questions (only 1 wrong answer)
      if (sizeof(questionData["incorrect_answers"]) == 1){
        if (questionData["correct_answer"] == "True"){ 
          triviaData[i, 2] = "A";
          triviaData[i, 3] = "True";
          }
        else { 
          triviaData[i, 2] = "B";
          triviaData[i, 3] = "False";
        }

        triviaData[i, 4] = ({"True", "False"});
      }

      else{
        formattedAnswers = allocate(4);
        // Shuffle
        shuffledAnswers = m_indices(mkmapping( 
          ({ questionData["correct_answer"]}) + questionData["incorrect_answers"]));

        //shuffledAnswers = m_indices(mkmapping(shuffledAnswers));
        
        // Find the right answer in the shuffled array
        for (int j=0; j<sizeof(shuffledAnswers); j++){
          if (shuffledAnswers[j] == questionData["correct_answer"]){
            switch(j) {
              case 0 : letterAnswer = "A"; break;
              case 1 : letterAnswer = "B"; break;
              case 2 : letterAnswer = "C"; break;
              case 3 : letterAnswer = "D"; break;
            }
          }
        }
        
        // Format
        for (int j=0; j<sizeof(shuffledAnswers); j++){
          formattedAnswers[j] = decode(shuffledAnswers[j]);
        }
        
        // Assign the values
        triviaData[i, 2] = letterAnswer;
        triviaData[i, 3] = decode(questionData["correct_answer"]);
        triviaData[i, 4] = formattedAnswers;

      }
    }
  }

  else {
    write("No results returned!");
  }

}

public int error_handler(){
  return notify_fail("Trivia API Error!\nContact someone who knows more than Bebop! (anyone)\n");
}

// Grab 50 questions at random. Any difficulty, any category.

public void get_questions(closure success, closure error) {
  success ||= #'question_handler;
  error ||= #'error_handler;

  mapping url_args = ([ "amount": 50 ]);
  
  // Uncomment to specify a category / difficulty
  // url_args["category"] = "General Knowledge";
  // url_args["difficulty"] = 2;

  load_object(PY_HTTP_CLIENT)->get_json(API_BASE_URL, url_args, 0, success, error);
}

public void update_trivia_data(){
  tell_object(this_player(), "Gathering data from the Trivia API. Please wait...\n");
  triviaData = ([]);
  get_questions(#'question_handler, #'error_handler);
}

// Clear the data before returning it ( no cheating! :) )

public mapping query_trivia_data(){
  mapping tempData = triviaData;
  // triviaData = ([]);
  return tempData;
}

// Kill the question handler
public int clean_up(int arg) { 
  this_object() && destruct(this_object()); return 0; }

public mapping get_dummy_data(){
  mapping dummyData = ([]);
  for (int i=0; i<50; i++){
    if(i<25){
      dummyData += 
        ([i: "Test Category";
        "This is a test question. The answer is B."; 
        "B";"Test2";
        ({"Test1","Test2", "Test3","Test4,"})]);
    }

    else {
      dummyData += 
        ([i: "Test Category";
        "This is a test question. The answer is B."; 
        "B";"Test2";
        ({"Test1","Test2"})]);
    }
  }

  return dummyData;
}
