<h3 style="color:gray">This project has been created as part the 42  by <span style="color:#28C775"> khnoman </span></h3> 
<h1 style="font-size: 30px; color: rgba(255, 255, 128, 0.5);">🧵⚙️💻 CODEXION</h1>

<br/>
<h2  style="font-size: 27px;">📋 Table of Contents</h2>
<ul>
<li><a href="#soverview"> Overview </a></li>
<li><a href="#Installation">Installation</a></li>
<li><a href="#Project_Description"> Project Description </a></li>
<li><a href="#Rules"> Rules </a></li>
<li><a href="#Project_Structure"> Project Structure </a></li>
<li><a href="#Blocking_Cases"> Blocking Cases Handled in This Project </a></li>
<li><a href="#Thread_Synchronization"> Thread Synchronization Mechanisms </a></li>
<li><a href="#Resources"> Resources </a></li>
<li><a href="#AI_Usage"> AI Usage </a></li>


</ul>
<h2 style="font-size: 27px;" id="overview"> 🎯 Overview </h2>
<p>
Codexion is a multithreaded CPU scheduling simulator written in C. It models how an operating system manages multiple threads competing for CPU time and shared resources.

The project explores concepts such as threads, mutexes, scheduling algorithms, synchronization, burnout detection, and deadlock prevention.
</p>

<h2 style="font-size: 27px;" id="Installation"> 🛠️ Installation</h2>

<h3> Clone The Repository</h3>
<h4>git clone <my_repo_link> codexion</h4>
<h3> Compile The Project</h3>
<h4>make</h4>
<h3> Run The Program</h3>
<h4>./codexion 4 1000 200 200 200 1 10 fifo</h4>
<h3>Output</h3>
<code>
2 1 has taken a dongle</br>
3 1 has taken a dongle</br>
3 1 is compiling</br>
7 3 has taken a dongle</br>
7 3 has taken a dongle</br>
7 3 is compiling</br>
273 3 is debuging</br>
273 1 is debuging</br>
273 4 has taken a dongle</br>
273 4 has taken a dongle</br>
274 4 is compiling</br>
274 2 has taken a dongle</br>
274 2 has taken a dongle</br>
276 2 is compiling</br>
474 1 is refactoring</br>
477 3 is refactoring</br>
478 2 is debuging</br>
478 4 is debuging</br>
712 4 is refactoring</br>
712 2 is refactoring</br>
</code>
<div>
<h2 style="font-size: 27px;"id="Project_Description"> 📖 Project Description</h2>
<p>
Codexion simulates a system with N coders and N dongles.
Each coder is represented by a thread. To start compiling, a coder must acquire two dongles (left and right). Since dongles are shared resources, mutexes are used to prevent conflicts.

A scheduler decides which coder is allowed to work according to the selected scheduling policy.

Supported scheduling policies:

FIFO (First In, First Out)
EDF (Earliest Deadline First)
</p>
</div>

<h2 style="font-size: 27px;" id="Rules"> 🎮 Rules </h2>
<h3>Allowed Operations: </h3>
<table border="1" cellpadding="8" cellspacing="0">
  <tr>
    <th>Function</th>
    <th>Purpose</th>
  </tr>

  <tr>
    <td>pthread_create</td>
    <td>Creates a new thread (starts a coder)</td>
  </tr>

  <tr>
    <td>pthread_join</td>
    <td>Waits for a thread to finish execution</td>
  </tr>

  <tr>
    <td>pthread_mutex_init</td>
    <td>Initializes a mutex (lock for shared resource)</td>
  </tr>

  <tr>
    <td>pthread_mutex_lock</td>
    <td>Locks a mutex (enter critical section)</td>
  </tr>

  <tr>
    <td>pthread_mutex_unlock</td>
    <td>Unlocks a mutex (leave critical section)</td>
  </tr>

  <tr>
    <td>pthread_mutex_destroy</td>
    <td>Destroys a mutex (free resources)</td>
  </tr>

  <tr>
    <td>pthread_cond_init</td>
    <td>Initializes a condition variable</td>
  </tr>

  <tr>
    <td>pthread_cond_wait</td>
    <td>Waits for a condition (releases mutex while waiting)</td>
  </tr>

  <tr>
    <td>pthread_cond_timedwait</td>
    <td>Waits for a condition with timeout</td>
  </tr>

  <tr>
    <td>pthread_cond_broadcast</td>
    <td>Wakes all waiting threads</td>
  </tr>

  <tr>
    <td>pthread_cond_destroy</td>
    <td>Destroys a condition variable</td>
  </tr>

  <tr>
    <td>gettimeofday</td>
    <td>Gets current time (used for timestamps & scheduling)</td>
  </tr>
</table>

<h2 style="font-size: 27px;" id="Project_Structure">📁 Project Structure </h2>

<pre style="font-family: 'Courier New', monospace; background: #7c817875; padding: 20px; border-radius: 5px;">
codexion/
├── codexion_code
|   ├── create_coders.c
|   ├── create_dongles.c
|   ├── f_h_lock_dongles.c
|   ├── f_h_thread_f.c
|   ├── f_h2_thread_f.c
|   ├── head.h
|   ├── heap.c
|   ├── main_helper.c
|   ├── main.c
|   ├── parsing.c
|   ├── timer_helper1.c
|   ├── timer_helper2.c
|   └──timer.c
├── Makefile
└── README.md
</pre>


<h3 style="font-size: 27px;" id="Blocking_Cases">Blocking Cases Handled in This Project </h3>

<div>
<h5>Deadlock Prevention</h5>
<span style="color:red">Definition:</span> Prevents coders from waiting forever for dongles.</br>
<p>
Steps:<br>
1. A coder requests dongles.<br>
2. Dongles are acquired using a safe strategy.<br>
3. Circular waiting is avoided.<br>
4. The simulation continues without deadlock.<br>

</p>
</div>

<div>
<h5>Starvation Prevention</h5>
<span style="color:red">Definition:</span> Ensures every coder eventually gets a chance to compile.
</br>
<p>
Steps:<br>
1. Runnable coders are added to the scheduler.<br>
2. The scheduler selects the next coder using FIFO or EDF.<br>
3. Selected coders compile and release resources.<br>
4. Waiting coders eventually get scheduled.<br>
</p>
</div>


<div>
<h5>Cooldown Handling</h5>
<span style="color:red">Definition:</span> After being released, a dongle becomes unavailable for a short cooldown period before another coder can use it.
</br>
<p>
Steps:<br>
1. A coder finishes using a dongle.<br>
2. The dongle is released.<br>
3. The dongle enters a cooldown period<br>
4. During this time, no other coder can use it.<br>
5. After the cooldown ends, the dongle becomes available again.<br>
</p>
</div>


<div>
<h5>Precise Burnout Detection</h5>
<span style="color:red">Definition:</span>Detects when a coder exceeds the allowed inactivity time.
</br>
<p>
Steps:<br>
1. The monitor checks all coders .<br>
2. It compares the current time with the last active time.<br>
3. If the limit is exceeded, burnout is detected.<br>
4. The simulation stops safely.<br>
</p>
</div>


<div>
<h5> Log Serialization</h5>
<span style="color:red">Definition:</span>Prevents messages from different threads from mixing.
</br>
<p>
Steps:<br>
1. A thread locks the print mutex.<br>
2. The message is printed.<br>
3. The mutex is unlocked.<br>
4. Another thread can print.<br>
</p>
</div>

<div>
<h5> Shared Data Protection</h5>
<span style="color:red">Definition:</span>Prevents multiple threads from modifying shared data simultaneously.
</br>
<p>
Steps:<br>
1. A thread locks a mutex.<br>
2. Shared data is accessed or modified.<br>
3. The operation completes.<br>
4. The mutex is unlocked.<br>
</p>
</div>

<div>
<h2 id ="Thread_Synchronization">🔒 Thread Synchronization Mechanisms</h2>
<h5 style="color:green">pthread_mutex_t</h5>

<h6>Mutexes protect shared resources and prevent race conditions.</h6>

<span style="color:yellow">Used for: </span>
<p>
Dongle ownership<br>
Console logging<br>
Shared scheduler data<br>
Monitor state<br>
Stop conditions<br>
</p>
<h4 style="color:green">Race Condition Prevention</h4>

<p>Before accessing shared data, a thread locks the corresponding mutex. After the operation is completed, the mutex is unlocked.</p>

<h4 style="color:green">pthread_cond_t (if used)</h4>

<p>Condition variables allow threads to sleep until a condition becomes true.<\p>

<p>They are used to avoid busy waiting and coordinate thread execution.</p>

<h4 style="color:green">Thread-Safe Communication</h4>

<p>Coders and the monitor communicate using shared variables protected by mutexes.</p>

<p>This guarantees consistent and safe access to shared state.</p>
</div>


<div>
<h2  style="font-size: 27px;" id="Resources"> 📚 Resources </h2>
<h4 style="font-size: 20px;"> Useful Links:</h4>
<ul>
<li> <a href="https://www.geeksforgeeks.org/dsa/introduction-to-min-heap-data-structure/"> introduction to min heap data structure</a> </li>
<li> <a href="https://www.geeksforgeeks.org/operating-systems/thread-in-operating-system/"> thread in operating system</a></li>
<li><a href="https://eng.libretexts.org/Courses/Delta_College/Operating_System:_The_Basics/04:_Threads/4.2:_Thread_Types">thread types<a/></li>
<li><a href="https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/#using-posix-threads">thread types<a/>threads</li>
<li><a href="https://www.youtube.com/watch?v=M9HHWFp84f0">video threads<a/></li>
<li><a href="https://www.youtube.com/watch?v=O2tV9q6784k&t=710s">video threads<a/></li>
<li><a href="https://www.tldraw.com/f/Fz6hc3-hn_mhDGBgKt_6-?d=v-3828.1861.19881.23146.page"> my project notes </a></li>
</ul>

<h3 id="AI_Usage">AI Usage</h3>
<ol>
<li>AI was used as a learning assistant to:</li>
<li>Understand operating system concepts.</li>
<li>Clarify pthread behavior.</li>
<li>Explore scheduling algorithms.</li>
</ol>
</div>