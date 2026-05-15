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
<li><a href="#Resources"> Resources </a></li>

</ul>
<h2 style="font-size: 27px;" id="overview"> 🎯 Overview </h2>
<p>
codexion is a multithreaded CPU scheduling simulator written in C , it models how an operationg system manages multiple threads competing for CPU time and shared resources.
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
Codexion is a multithreaded project with N coders and N dongles.
Each coder needs two dongles (left and right) to start working.
Coders are threads and they share dongles using mutex to avoid conflict.
The scheduler decides which coder works and when.
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

///
</pre>
<h2  style="font-size: 27px;" id="Resources"> 📚 Resources </h2>
<h4 style="font-size: 20px;"> Useful Links:</h4>
<ul>
<li> <a href="https://42-cursus.gitbook.io/guide/2-rank-02/push_swap"> Push_swap Tutorial (42 Docs)</a> </li>
<li> <a href="https://www.geeksforgeeks.org/dsa/stack-data-structure/"> Stack Data Structure</a></li>
<li><a href="https://www.youtube.com/watch?v=wRvipSG4Mmk&themeRefresh=1">push_swap tutorial video<a/></li>
</ul