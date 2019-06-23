# json_streamer

Requirements:

* Boost 1.6.9 filesystem (unit_test_framework required for testapp)
* C++ 17
* Cmake > 3.12


Example:


JsonStreamer<Object, int> streamer(path, keys);

Object => struct of some kind
int => value type in struct

e.g. struct Object { int data1; int data2; };


path => absolute path to json file to be written and read

keys => default type is forward_list<std::string>, 
can be changed but is not tested:<br>
    example: JsonStreamer<Object, int, std::vector<std::string>> streamer(path, keys);


streamer.write(value) //  => instance of struct<br>
    example: streamer.write(obj) 

streamer.fetch(value) // value of head of json object<br>
    example: streamer.fetch("parent2") // se example file below


Exempel json file:
<pre><code>
{
    "0": {
        "head": "parent",
        "data": {
            "id": "1",
            "description": "testing",
            "value": "34"
        }
    },
     "1": {
        "head": "parent2",
        "data": {
            "id": "2",
            "description": "testing",
            "value": "9"
        }
     }
}
</code></pre>
