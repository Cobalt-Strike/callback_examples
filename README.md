
# Cobalt Strike Aggressor Script Callback Functions

With the Cobalt Strike release 4.9 support for using callbacks were added to
the following Aggressor Script functions:
- bnet
- beacon_inline_execute
- binline_execute
- bdllspawn
- bexecute_assembly
- bhashdump
- bmimikatz
- bmimikatz_small
- bportscan
- bpowerpick
- bpowershell
- bpsinject

In this public project there are examples for each of these functions.  These
are simple examples which are meant to give users a jump start on how a
callback function can be useful for your red team operations.

The examples use the following style:
- All commands start with 'cs_'
- All commands are registered using the beacon_command_register function
- All commands that accept arguments use this style: `--<arg> <value>`
- All variables are declared in either global or local scope.
- Debug information via `println("<some info>");` are commented out.

As mentioned most of the examples are very simple, however there is one
example that demonstrates how a callback and event listener can be used
to automate something that would normally require an operator to perform
multiple steps.  This example is the `cs_export_certificates` command
which uses the mimikatz module to export certificates, parse the output
looking for filenames and download them, and once they are completely
downloaded will remove them from the target.

# What is a Callback

A callback is used to allow the user to get access to the result and do
additional processing on the information.  Cobalt Strike and Aggressor
Script uses the concept of callbacks because of the asynchronous behavior
of sending a task to beacon and the response being received sometime in the
future based on the current sleep time.

Once your asynchronous callback is executed you will be able to perform the
necessary operations to process the result for your use case.  Here are some
examples of what you can do with the result.
- Format the result before displaying in the Beacon Console
- Scan the result for information to trigger some additional task
- Save the information to a file

A callback function will have arguments and in most cases will have the same
arguments, however there are some exceptions.  You should always refer to the
aggressor script function documentation to understand what arguments are
being passed to your callback.

### Callback Request and Response Processing

The following describes at a high level what goes on when a callback is used in
an aggressor script command.

- The client executes an aggressor script command with a callback
  - A request is created and saved in a queue to be retrieved later
  - The request is sent to the teamserver
- The teamserver receives the request
  - The request is saved in a queue to be retrieved later
  - The request is sent to a beacon
- The Beacon receives the request and processes the task
  - A response is generated and sent to the teamserver
- The teamserver receives the response
  - The request is retrieved from the teamserver queue using an id from the response
  - A reply is generated and sent to the originating client
- The originating client receives the response
  - The request is retrieved from the client queue using an id from the response
  - The client will execute the callback

Both the client and teamserver save requests that have associated callbacks in a
queue.  A request is eventually removed in order to maintain the number of
request in the queue.  A request is removed when these two conditions occur.

The first condition is when the originating client disconnects from the teamserver.
When this happens the queue managed by the client is removed as the queue is per
teamserver connection.  The queue on the teamserver will see the originating
client has disconnected and flag any requests for that client to be removed.
This means the originating client needs to stay connected to the teamserver
until the command with a callback has completed.  Otherwise, any responses from
Beacon after a disconnection from the originating client will be lost.

The second condition is when there is no responses for a request after a period
of time.  There are two timeout settings that determine if a request should be
removed.  The first setting is the limits.callback_max_timeout which defaults
to 1 day, which is used to wait for the initial response.  The second setting
is the limits.callback_keep_timeout which defaults to 1 hour, which is used to
wait for subsequent responses.  These settings can be modified by updating the
TeamServer.prop file. In most use cases the defaults should be fine, however
if you create a command that is a long-running job/task then these settings
may need to be adjusted.  The adjusted settings need to be based on how often
data will be received, which needs to account for beacon's sleep time and how
often the job/task sends data.

If you see error(s) like the following in the teamserver console window then
this can indicate the settings need to be adjusted or the originating client
has disconnected from the teamserver.

`"Callback #/# has no pending request"`

The TeamServer.prop file is not included in the Cobalt Strike distribution.  The
current default file can be found at https://github.com/Cobalt-Strike/teamserver-prop
repository.

### Callback Implementation

Aggressor script callbacks can be implemented using a few different techniques
and in many cases the technique used is based on your personal preference.
There are some use cases where you will want to choose a particular technique
in order to accomplish the task.  The following are types of techniques that
can be used followed by simple snippets of code:
- Anonymous Closure
- Named Closure
- Lambda Closure

#### Anonymous Closure Example

An anonymous closure is useful when you have a small amount of code that
can be kept inline with the caller.  In this example the closure is executed
in the future when data is returned from a BOF, which simply logs the output
to the beacon console.

    alias cs_example {
        # User setup code removed for brevity
        beacon_inline_execute($bid, $data, "go", $args, { blog($1, $2); });
    }

#### Named Closure Example

A named closure is useful when you have a lot of code and may want to reuse
the code with other aggressor functions.  In this example the closure named
`bof_cb` is executed in the future when data is returned from a BOF.

    # $1 - bid, $2 - result, $3 - info map
    sub bof_cb {
        # User defined code removed for brevity
    }
    alias cs_example {
        local('$bid $data $args');
        # User setup code removed for brevity
        beacon_inline_execute($bid, $data, "go", $args, &bof_cb);
    }

#### Lambda Closure Example

A lambda closure is useful when you want to pass variable(s) that would not
be in scope using the previous methods.  This example shows how you can
get access to the $test_num variable which is in the scope of the cs_example
alias.

    # $1 - bid, $2 - result, $3 - info map, $4 - test_num
    sub bof_cb {
        # User defined code removed for brevity
    }
    alias cs_example {
        local('$bid $file $test_num');
        # User setup code removed for brevity
        binline_execute($bid, $file, $test_num, lambda({ bof_cb($1, $2, $3, $test_num); }, \$test_num);
    }

#### File and Directory Descriptions

Location                             | Description
-------------------------------------|------------
cs_main.cna                          | Main CNA script to load all other script files
bnet                                 | Contains example scripts using the bnet aggressor function
bof/beacon_inline_execute            | Contains example script using the beacon_inline_execute aggressor function
bof/binline_execute                  | Contains example script using the binline_execute aggressor function
dllspawn/example_dll                 | Contains example of a reflectively loaded dll using the bdllspawn aggressor function
execute_assembly/example_ea          | Contains example of a .NET executable assembly using the bexecute_assembly aggressor function
hashdump                             | Contains example script using the bhashdump aggressor function
mimikatz/coffee                      | Contains example script using the bmimikatz aggressor function
mimikatz/export_certificates         | Contains example script using the bmimikatz aggressor function
mimikatz/logon_passwords             | Contains example script using the bmimikatz_small aggressor function
portscan                             | Contains example script using the bportscan aggressor function
powershell/bpowerpick                | Contains example script using the bpowerpick aggressor function
powershell/bpowershell               | Contains example script using the bpowershell aggressor function
powershell/psinject                  | Contains example script using the psinject aggressor function

## Usage

To use these examples, load the cs_main.cna into Cobalt Strike.

- (Optional) Review the code files and rebuild any binaries
- (Optional) Open the Script console, Cobalt Strike -> Script Console
- Open the Script manager, Cobalt Strike -> Script Manager
- Load `/path/to/cs_main.cna`
- Open a Beacon Console
- Enter `help` in the Beacon Console and look for commands starting with 'cs_'
- Enter `help <command>` in the Beacon Console and see information about the `<command>`
- Execute the `<command>` based on the usage examples and see how it works

### References

- https://hstechdocs.helpsystems.com/manuals/cobaltstrike/current/userguide/content/topics/agressor_script.htm
- http://sleep.dashnine.org/manual/index.html
