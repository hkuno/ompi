Getting Help
============

If you have a problem or question, it is highly recommended that you
execute the following steps **in order**.  Many people have similar
problems with configuration and initial setup of Open MPI -- it is
possible that your question has already been answered.

#. :doc:`Check the Open MPI FAQ </faq/index>`.  We generally take the
   approach that if the same question comes up multiple times, we put
   it on the FAQ.

#. `Check the mailing list archives
   <https://www.open-mpi.org/community/lists/ompi.php>`_ "search"
   features (or use Google) to check old posts and see if others have
   asked the same question and had it answered.

#. If you do not find a solution to your problem in the above
   resources, send the following information to the `Open MPI user's
   mailing list <https://www.open-mpi.org/community/lists/ompi.php>`_.

   .. note:: The mailing lists have *a 150 KB size limit on messages*
             If attaching your files results in an email larger than
             this, please try compressing it and/or posting it on the
             web somewhere for people to download.  A `Github Gist
             <https://gist.github.com/>`_ or a `Pastebin
             <https://pastebin.com/>`_ might be an easy choice for
             posting large text files.

  .. important:: The more information you include in your report, the
                 better.  E-mails simply stating, "It doesn't work!"
                 are not helpful; we need to know as much information
                 about your environment as possible in order to
                 provide meaningful assistance.  **The best way to get
                 help** is to provide a "recipie" for reproducing the
                 problem.  This will allow the Open MPI developers to
                 see the error for themselves, and therefore be able
                 to fix it.

  .. important:: Please **use a descriptive "subject" line in your
                 email!** Some Open MPI question-answering people
                 decide whether to read a mail based on its subject
                 line (e.g., to see if it's a question that they can
                 answer).  So please plese please use a good subject
                 line that succinctly describes your problem.

/////////////////////////////////////////////////////////////////////

Where to send?
--------------

Different types of questions and problems should be sent to different
places.  If you have:

* A general end-user question or problem, you should probably
  subscribe to the `Open MPI user's mailing list
  <https://www.open-mpi.org/community/lists/ompi.php>`_ and post it
  there.

  * If you have a run-time question or problem, see the :ref:`For
    run-time problems <getting-help-run-time-label>` section below for
    the content of what to include in your email.
  * If you have a compile-time question or problem, see the :ref:`For
    compile-time problems <getting-help-compile-time-label>` section
    below for the content of what to include in your email.

* A bug report, you should probably post it to `Open MPI's Github
  issue tracker <https://github.com/open-mpi/ompi/issues>`_.  Follow
  the template to submit all the requested information.

* A patch, bug fix, or other code submission, please post a Github
  Pull Request to the `Open MPI Github repository
  <https://github.com/open-mpi/ompi/pulls>`_.

* A developer-level / internal question about Open MPI itself, you
  should probably subscribe to the `Open MPI developer's mailing list
  <https://www.open-mpi.org/community/lists/ompi.php>`_ and post it
  there.

/////////////////////////////////////////////////////////////////////

.. _getting-help-run-time-label:

For run-time problems
---------------------

First, reach / search through this documentation.  Really.  This can
save you a *lot* of time; many common problems and solutions are
listed here.

If you don't find the answer here, provide *all* of the following
information:

#. The version of Open MPI that you're using.

#. The ``config.log`` file from the top-level Open MPI directory, if
   available (**compress with or post to a Github gist or Pastebin**).

#. The output of the ``ompi_info --all`` command from the node where
   you're invoking ``mpirun``.

#. If you have questions or problems about process affinity /
   binding, send the output from running the ``lstopo -v``
   command from a recent version of `Hwloc
   <https://www.open-mpi.org/projects/hwloc/>`_.  *The detailed
   text output is preferable to a graphical output.*

#. If running on more than one node -- especially if you're
   having problems launching Open MPI processes -- also include
   the output of the ``ompi_info --version``
   command **from each node on which you're trying to run**.

   #. If you are able to launch MPI processes, you can use
      ``mpirun`` to gather this information.  For example, if
      the file ``my_hostfile.txt`` contains the hostnames of the
      machines on which you are trying to run Open MPI
      processes:

      .. code-block:: sh
         :linenos:

         shell$ mpirun --bynode --hostfile my_hostfile.txt --tag-output ompi_info --version


   #. If you cannot launch MPI processes, use some other mechanism --
      such as ``ssh`` -- to gather this information.  For example, if
      the file ``my_hostfile.txt`` contains the hostnames of the
      machines on which you are trying to run Open MPI processes:

      .. code-block:: sh
         :linenos:

         # Bourne-style shell (e.g., bash, zsh, sh)
         shell$ for h in `cat my_hostfile.txt`
         > do
         > echo === Hostname: $h
         > ssh $h ompi_info --version
         > done

      .. code-block:: sh
         :linenos:

         # C-style shell (e.g., csh, tcsh)
         shell% foreach h (`cat my_hostfile.txt`)
         foreach? echo === Hostname: $h
         foreach? ssh $h ompi_info --version
         foreach? end

#. A *detailed* description of what is failing.  The more
   details that you provide, the better.  E-mails saying "My
   application doesn't work!" will inevitably be answered with
   requests for more information about *exactly what doesn't
   work*; so please include as much information detailed in your
   initial e-mail as possible.  We strongly recommend that you
   include the following information:

   * The exact command used to run your application.

   * Any relevant MCA parameters that were set (or unset) when
     you ran (from either the command line, environment,
     parameter file, etc.).

   * The value of the ``PATH`` and ``LD_LIBRARY_PATH``
     environment variables (did you set them correctly to point
     to all relevant executables, the Open MPI libraries, and
     any required support libraries, such as libraries required
     for high-speed networks such as InfiniBand).

#. Detailed information about your network:

   #. For InfiniBand-based networks, include the information
      :ref:`in this FAQ entry <faq-ib-troubleshoot-label>`.

   #. For TCP-based networks, include the output of the
      ``ifconfig`` command on all relevant nodes.  Note that
      some Linux distributions do not put ``ifconfig`` in the
      default ``PATH`` of normal users; look for it in
      ``/sbin/ifconfig`` or ``/usr/sbin/ifconfig``.

/////////////////////////////////////////////////////////////////////

.. _getting-help-compile-time-label:

For compile problems
--------------------

First, reach / search through this documentation.  Really.  This can
save you a *lot* of time; many common problems and solutions are
listed there.

If you don't find the answer there, provide *all* of the following
information:

#. The version of Open MPI that you're using.

#. All output (both compilation output and run time output, including
   all error messages).

#. Output from when you ran ``./configure`` to configure Open MPI
   (**compress or post to a GitHub gist or Pastebin!**).

#. The ``config.log`` file from the top-level Open MPI directory
   (**compress or post to a GitHub gist or Pastebin!**).

#. Output from when you ran ``make V=1`` to build Open MPI (**compress
   or post to a GitHub gist or Pastebin!**).

#. Output from when you ran ``make install`` to install Open MPI
   (**compress or post to a GitHub gist or Pastebin!**).

To capture the output of the configure and make steps, you can use the
script command or the following technique to capture all the files in
a unique directory, suitable for tarring and compressing into a single
file:

.. code-block:: sh
   :linenos:

   # Bourne-style shell (e.g., bash, zsh, sh)
   shell$ mkdir $HOME/ompi-output
   shell$ ./configure {options} 2>&1 | tee $HOME/ompi-output/config.out
   shell$ make all 2>&1              | tee $HOME/ompi-output/make.out
   shell$ make install 2>&1          | tee $HOME/ompi-output/make-install.out
   shell$ cd $HOME
   shell$ tar jcvf ompi-output.tar.bz2 ompi-output

.. code-block:: sh
   :linenos:

   # C-style shell (e.g., csh, tcsh)
   shell% mkdir $HOME/ompi-output
   shell% ./configure {options} |& tee $HOME/ompi-output/config.out
   shell% make all              |& tee $HOME/ompi-output/make.out
   shell% make install          |& tee $HOME/ompi-output/make-install.out
   shell% cd $HOME
   shell% tar jcvf ompi-output.tar.bz2 ompi-output

Then send the resulting ``ompi-output.tar.bz2`` file to the mailing
list.

.. note:: The mailing lists have a *150KB size limit* on attachments.
   If your post is rejected because the attachment is too large, you
   might want to post the files somewhere on the internet (e.g., a
   GitHub gist or a Pastebin) and just send links in your post to the
   mailing list.

Make today an Open MPI day!
