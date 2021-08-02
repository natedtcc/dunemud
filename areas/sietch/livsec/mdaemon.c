int count = 0;
query_count() { return count; }

dec_count() 
{ 
  count--;
//  tell_object(find_living("bebop"),"counts ir :"+count); 
  return 1;
}

inc_count() 
{ 
//  tell_object(find_living("bebop"),"counts ir :"+count); 
  return 1;
}

