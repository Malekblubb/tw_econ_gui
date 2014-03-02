Teeworlds External Console - GUI
================================

This is a graphical client that can access the Telnet/Econ api of a teeworlds server.
You can execute any command that the server supports via a graphical interface.
With the "Jobs" feature you can create jobs that will be automatically executed
after a given time. It is also possible to let the job repeat after it gets
executed.


### Server setup
You must add the following server settings to your server-config-file:
`ec_port 4444`: The port which the econ will use (e.g.  4444)
`ec_password test123`: The password (bad example: test123)
`ec_bantime 60`: The bantime when entering the password wrong
`ec_auth_timeout 30`: The time before the auth times out
`ec_output_level 2`: The amount of log-information in the econ

If you are not sure, just copy all the settings to your config and
only change `ec_port` and `ec_password`.


### Screenshots

#### Linux
![img](http://37.221.194.71/img/twec0.png)


#### Windows
![img](http://37.221.194.71/img/twec1.png)
