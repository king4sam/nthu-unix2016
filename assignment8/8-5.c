typedef struct pbt
{
  pthread_mutex_t lock;
  pthread_cond_t  cond;
  int count;
  int finished;
}mpthread_barrier_t;

int mpthread_barrier_wait(mpthread_barrier_t *barrier){
  int reval;
  pthread_t self = pthread_self();
  pthread_mutex_lock(&barrier->lock);

  barrier->finished++;
  //all thread finished
  //signal all waiting thread
  if (barrier->count == barrier->finished) {
    pthread_cond_broadcast(&barrier->cond);
    retval = PTHREAD_BARRIER_SERIAL_THREAD;
  }
  //wait for cond
  else {
    reval = pthread_cond_wait(&barrier->cond, &barrier->lock);
  }
  pthread_mutex_unlock(&barrier->lock);
  return reval;
}

int
mpthread_barrier_init(mpthread_barrier_t *barrier, unsigned int count) {
  int err;

  barrier->count = count;
  barrier->finished = 0;

  err = pthread_mutex_init(&barrier->lock, NULL);
  if (err != 0)
    return err;
  err = pthread_cond_init(&barrier->cond, NULL);
  if (err != 0)
    return err;


  return 0;
}