^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package odva_ethernetip
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.1.4 (2019-02-21)
------------------

* Update .travis.yml to use industrial CI to test against Xenial
* Logging using console_bridge (`#15 <https://github.com/ros-drivers/odva_ethernetip/issues/15>`_)
  - cout replaced by logInform
  - cerr with 'Warning: ' string replaced by logWarn
  - cerr without 'Warning: ' string replaced by logErr
* Contributors: Rein Appeldoorn

0.1.3 (2018-04-13)
------------------
* Make sure to #include <boost/array.hpp> in the header file.
* Switch to unsigned char arrays in the tests.
  This gets rid of a narrowing error thrown by the compiler.
* Contributors: Chris Lalancette

0.1.2 (2018-04-12)
------------------
* Optional local_ip (`#8 <https://github.com/ros-drivers/odva_ethernetip/issues/8>`_)
* Exclude tests from rosdoc.
* Minor manifest and build script updates.
* Use ros-shadow-fixed for dependencies.
* Contributors: Mike Purvis, Rein Appeldoorn, gavanderhoorn

0.1.1 (2015-11-17)
------------------
* Fixed library name.
* Contributors: Tony Baltovski

0.1.0 (2015-11-17)
------------------
* Initial release.
