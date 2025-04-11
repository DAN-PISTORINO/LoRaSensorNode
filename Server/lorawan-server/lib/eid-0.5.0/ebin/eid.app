{application,eid,
             [{description,"Unique ID generator"},
              {id,"eid"},
              {vsn,"0.5.0"},
              {modules,[eid,eid_app,eid_server,eid_sup,eid_utils]},
              {registered,[eid_sup,eid_server]},
              {applications,[kernel,stdlib]},
              {mod,{eid_app,[]}},
              {env,[]}]}.
