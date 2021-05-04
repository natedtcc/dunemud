// lifetime.c - Manage lifetime of an object - Bebop 2020
// Thanks to Bubbs for the help!

int     lifespan_time         = 0;
closure lifespan_check        = 0;
closure lifespan_complete     = 0;

// Check lifespan, destroy object, generate new object

void check_lifespan() {
  if (lifespan_time && (time() > lifespan_time) && funcall(lifespan_check)){
    call_out((: this_object() && funcall(lifespan_complete)
    && destruct(this_object()) :), 1);
  }
}

void heart_beat() { check_lifespan(); }

int query_lifespan_time() { return lifespan_time; }

void set_lifespan(int lifetime, mixed check, mixed complete) {
  lifespan_time                 = time() +(lifetime || 20);
  lifespan_check                = check;
  lifespan_complete             = complete;

  lifetime && set_heart_beat(1);
}

int check_lifetime() {
  if (!environment());
  else if (roomp(environment()))
    tell_others(this_object(), 
      "A shady-looking sapho smuggler runs in and grabs "
      +"a keycard off of the ground!\n");
  else {
    tell_object(environment(),
      "A shady-looking sapho smuggler runs in and steals your keycard!\n");
    tell_others(environment(),
    "A shady-looking sapho smuggler runs in and steals %N's keycard!\n");
  }

  return 1;
}

int complete_lifetime()  {

  return 1;

}