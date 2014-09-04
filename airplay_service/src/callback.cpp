#include "callback.hpp"
#include <iostream>
#include "util.hpp"
#include "structs.h"
#include "pugixml.hpp"
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <boost/any.hpp>
#include <netinet/in.h>
#include "Plist.hpp"
#include "net.hpp"

using std::cout;
using std::vector;

std::map<string, string> params_map;
enum state_param {KEY, VALUE};
state_param state_params  =   KEY;
string last_key           =   "";

//A client can fetch the list of available transitions for slideshows.
void get_slideshow_features(const int &sock)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
    string  key             =   "Reflections";
    string  name            =   "Reflections";

    string  msg_reponse     =   "";
    msg_reponse             =   msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                                + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                                + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                                + "<plist version=\"1.0\">" + "\n"
                                + "<dict>" + "\n"
                                + "<key>themes</key>" + "\n"
                                + "<array>" + "\n"
                                + "<dict>" + "\n"
                                + "<key>key</key>" + "\n"
                                + "<string>" + key + "</string>" + "\n"
                                + "<key>name</key>" + "\n"
                                + "<string>" + name + "</string>" + "\n"
                                + "</dict>" + "\n"
                                + "</array>" + "\n"
                                + "</dict>" + "\n"
                                + "</plist>" + "\n";

    const char *msg         =   msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Send a JPEG picture to the server.
void put_photo(const int &sock, const Request &rq)
{
    http_response_t           *res                   =   http_response_init("HTTP/1.1", 200, "OK");
    std::map<string, string>  map                    =   rq.get_headers_map();
    string                    x_apple_assetKey       =   map["X-Apple-AssetKey"];
    string                    x_apple_transition     =   map["X-Apple-Transition"];
    string                    x_apple_assetAction    =   map["X-Apple-AssetAction"];

    // Data photo contained in rq.data;

    if (x_apple_transition == "Dissolve")
    {
        //if picture using the dissolve transition
        print_debug("Picture using the dissolve transition\n");
    }
    else
    {
        //if show a picture without any transition
        print_debug("Show a picture without any transition\n");
        // print_debug("%s\n", rq.get_data());
    }

    if (x_apple_assetAction == "cacheOnly")
    {
        //cache a picture for future display
        print_debug("Cache a picture for future display\n");
    }
    else if (x_apple_assetAction == "displayCached")
    {
        //show a cached picture
        print_debug("Show a cached picture\n");
    }

    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Start or stop a slideshow session.
void put_slideshow_session(const int &sock, const Request &rq)
{
    map<string, boost::any> dict;
    Plist::readPlist(rq.get_data(), rq.get_len_data(), dict);
    const map<string, boost::any> &dictSettings = boost::any_cast<const map<string, boost::any>&>(dict.find("settings")->second);

    int     slideDuration   =   boost::any_cast<const int64_t &>(dictSettings.find("slideDuration")->second);
    string  theme           =   boost::any_cast<const string &>(dictSettings.find("theme")->second);
    string  state           =   boost::any_cast<const string &>(dict.find("state")->second);

    print_debug("slideDuration = %d\n", slideDuration);
    print_debug("Theme = %s\n", theme.c_str());
    print_debug("State = %s\n", state.c_str());

    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
    string  msg_reponse     =  "";
    msg_reponse             =  msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                               + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                               + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                               + "<plist version=\"1.0\">" + "\n"
                               + "<dict/>" + "\n"
                               + "</plist>" + "\n";
    const char *msg         =   msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//notifies a client that a photo session has ended.
void post_event_photo(const int &sock)
{
    string x_apple_session_ID   =   "1bd6ceeb-fffd-456c-a09c-996053a7a08c";
    string category             =   "photo";
    string sessionID            =   "38";
    string state                =   "stopped";

    http_request_t *req         =   http_request_init("HTTP/1.1", "POST", "/event");
    http_request_add_header(req, "Content-Type", "text/x-apple-plist+xml");
    http_request_add_header(req, "X-Apple-Session-ID", x_apple_session_ID.c_str());
    string msg_request          =   "";
    msg_request                 =   msg_request + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                                    + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                                    + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                                    + "<plist version=\"1.0\">" + "\n"
                                    + "<dict>" + "\n"
                                    + "<key>category</key>" + "\n"
                                    + "<string>" + category + "</string>" + "\n"
                                    + "<key>sessionID</key>" + "\n"
                                    + "<integer>" + sessionID + "</integer>" + "\n"
                                    + "<key>state</key>" + "\n"
                                    + "<string>" + state + "</string>" + "\n"
                                    + "</dict>" + "\n"
                                    + "</plist>" + "\n";
    const char *msg             =   msg_request.c_str();
    http_request_finish(req, msg, strlen(msg));
    send_req_to_socket(sock, req);
    http_request_destroy(req);

    http_parse_from_socket(sock);
}

// fetch a new picture
void get_slideshow(const int &sock)
{
    string id                   =   "1";
    string key                  =   "1";
    string url                  =   "";
    url                         +=  "/slideshows/" + id + "/assets/" + key;
    string x_apple_session_ID   =   "1bd6ceeb-fffd-456c-a09c-996053a7a08c";

    http_request_t *req         =   http_request_init("HTTP/1.1", "GET", url.c_str());
    http_request_add_header(req, "Accept", "application/x-apple-binary-plist");
    http_request_add_header(req, "X-Apple-Session-ID", x_apple_session_ID.c_str());

    http_request_finish(req, NULL, 0);
    send_req_to_socket(sock, req);
    http_request_destroy(req);
    http_parse_from_socket(sock);
}

// Fetch general informations about the AirPlay server.
void get_server_info(const int &sock)
{
    string deviceid     =   "58:55:CA:1A:E2:88";
    string features     =   "14839";
    string model        =   "AppleTV2,1";
    string protovers    =   "1.0";
    string srcvers      =   "120.2";

    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
    string msg_reponse  =   "";
    msg_reponse         =   msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                            + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                            + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                            + "<plist version=\"1.0\">" + "\n"
                            + "<dict>" + "\n"
                            + "<key>deviceid</key>" + "\n"
                            + "<string>" + deviceid + "</string>" + "\n"
                            + "<key>features</key>" + "\n"
                            + "<integer>" + features + "</integer>" + "\n"
                            + "<key>model</key>" + "\n"
                            + "<string>" + model + "</string>" + "\n"
                            + "<key>protovers</key>" + "\n"
                            + "<string>" + protovers + "</string>" + "\n"
                            + "<key>srcvers</key>" + "\n"
                            + "<string>" + srcvers + "</string>" + "\n"
                            + "</dict>" + "\n"
                            + "</plist>" + "\n";
    const char *msg     =   msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}
void post_play(const int &sock, const Request &rq)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    print_debug("Content-Type: %s\n", rq.get_headers_map()["Content-Type"].c_str());

    if (rq.get_headers_map()["Content-Type"] == "application/x-apple-binary-plist")
    {
        params_map.clear();

        map<string, boost::any> dict;
        Plist::readPlist(rq.get_data(), rq.get_len_data(), dict);
        string content_location   =   boost::any_cast<const string &>(dict.find("Content-Location")->second);
        double start_position     =   boost::any_cast<const double &>(dict.find("Start-Position")->second);

        print_debug("Content-Location = %s\n", content_location.c_str());
        print_debug("Start-Position = %f\n", start_position);
    }
    else
    {
        print_debug("data: %s\n", rq.get_data());
        params_map.clear();
        msgs_map_str_parse(params_map, rq.get_data());

        print_debug("Content-Location = %s\n", params_map["Content-Location"].c_str());
        print_debug("Start-Position = %f\n", params_map["Start-Position"].c_str());
    }

    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Seek at an arbitrary location in the video.
void post_scrub(const int &sock, const char *argument)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    params_map.clear();
    attrs_quotes_map_str_parse(params_map, argument);
    print_debug("Seek to position: %s\n", params_map["position"].c_str());
    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Change the playback rate.
void post_rate(const int &sock, const char *argument)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    params_map.clear();
    attrs_quotes_map_str_parse(params_map, argument);
    print_debug("Change playback rate: %s\n", params_map["value"].c_str());
    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Stop playback.
void post_stop(const int &sock)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    //Stop a photo or slideshow session.
    print_debug("Stop playback\n");
    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Retrieve the current playback position.
void get_scrub(const int &sock)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    http_response_add_header(res, "Content-Type", "text/parameters");
    //add duration here
    string duration         =   doubletostr(15);
    string position         =   doubletostr(1);

    string msg_reponse      =   "";
    msg_reponse = msg_reponse + "duration: " + duration + "\nposition: " + position;
    print_debug("msg_reponse: %s\n", msg_reponse.c_str());
    const char *msg         =   msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
    send_res_to_socket(sock, res);
    http_response_destroy(res);

}

//Retrieve playback informations such as position, duration, rate, buffering status and more.
void get_playback_info(const int &sock)
{
    http_response_t *res                =   http_response_init("HTTP/1.1", 200, "OK");
    //get playback access log
    string duration                     =   "1801";
    string loadedTimeRanges_duration    =   "51.541130402";
    string loadedTimeRanges_start       =   "18.118717650000001";
    string playbackBufferEmpty          =   "true";
    string playbackBufferFull           =   "false";
    string playbackLikelyToKeepUp       =   "true";
    string position                     =   "18.043869775000001";
    string rate                         =   "1";
    string readyToPlay                  =   "true";
    string seekableTimeRanges_duration  =   "1801";
    string seekableTimeRanges_start     =   "0.0";

    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
    // cout << params_map["theme"] << "\n";
    string msg_reponse  =   "";
    msg_reponse         =   msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                            + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                            + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                            + "<plist version=\"1.0\">" + "\n"
                            + "<dict>" + "\n"
                            + "<key>duration</key> <real>" + duration + "</real>" + "\n"
                            + "<key>loadedTimeRanges</key>" + "\n"
                            + "<array>" + "\n"
                            + "<dict>" + "\n"
                            + "<key>duration</key> <real>" + loadedTimeRanges_duration + "</real>" + "\n"
                            + "<key>start</key> <real>" + loadedTimeRanges_start + "</real>" + "\n"
                            + "</dict>" + "\n"
                            + "</array>" + "\n"
                            + "<key>playbackBufferEmpty</key> <" + playbackBufferEmpty + "/>" + "\n"
                            + "<key>playbackBufferFull</key> <" + playbackBufferFull + "/>" + "\n"
                            + "<key>playbackLikelyToKeepUp</key> <" + playbackLikelyToKeepUp + "/>" + "\n"
                            + "<key>position</key> <real>" + position + "</real>" + "\n"
                            + "<key>rate</key> <real>" + rate + "</real>" + "\n"
                            + "<key>readyToPlay</key> <" + readyToPlay + "/>" + "\n"
                            + "<key>seekableTimeRanges</key>" + "\n"
                            + "<array>" + "\n"
                            + "<dict>" + "\n"
                            + "<key>duration</key>" + "\n"
                            + "<real>" + seekableTimeRanges_duration + "</real>" + "\n"
                            + "<key>start</key>" + "\n"
                            + "<real>" + seekableTimeRanges_start + "</real>" + "\n"
                            + "</dict>" + "\n"
                            + "</array>" + "\n"
                            + "</dict>" + "\n"
                            + "</plist>" + "\n";
    const char *msg     =   msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
    send_res_to_socket(sock, res);
    http_response_destroy(res);

}

//Set playback property.
void put_setProperty(const int &sock, const char *argument, const Request &rq)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    map<string, boost::any> dict;
    Plist::readPlist(rq.get_data(), rq.get_len_data(), dict);
    const map<string, boost::any> &dictValue = boost::any_cast<const map<string, boost::any>&>(dict.find("value")->second);

    int epoch       =   boost::any_cast<const int64_t &>(dictValue.find("epoch")->second);
    int flags       =   boost::any_cast<const int64_t &>(dictValue.find("flags")->second);
    int timescale   =   boost::any_cast<const int64_t &>(dictValue.find("timescale")->second);
    int value       =   boost::any_cast<const int64_t &>(dictValue.find("value")->second);

    print_debug("epoch = %d\n", epoch);
    print_debug("flags = %d\n", flags);
    print_debug("timescale = %d\n", timescale);
    print_debug("value = %d\n", value);

    int errorCode = 0;

    if (!strcmp(argument, "forwardEndTime"))
    {
        //set forward end time
    }
    else if (!strcmp(argument, "reverseEndTime"))
    {
        //set reverse end time
    }

    http_response_add_header(res, "Content-Type", "application/x-apple-binary-plist");

    map<string, boost::any> dictRes;
    dictRes["errorCode"]  =   int64_t(error);

    vector<char> msg;
    Plist::writePlistBinary(msg, dictRes);
    http_response_finish(res, reinterpret_cast<char *> (&msg[0]), msg.size());
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Get playback property.
void get_getProperty(const int &sock, const char *argument)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");
    map<string, boost::any> dictRes;

    if (!strcmp(argument, "playbackAccessLog"))
    {
        //get playback access log
        int     errorCode                       =   0;
        int     value_bytes                     =   1818336;
        double  value_c_duration_downloaded     =   70;
        double  value_c_duration_watched        =   18.154102027416229;
        int     value_c_frames_dropped          =   0;
        double  value_c_observed_bitrate        =   14598047.302367469;
        int     value_c_overdue                 =   0;
        int     value_c_stalls                  =   0;
        double  value_c_start_time              =   0.0;
        double  value_c_startup_time            =   0.27732497453689575;
        string  value_cs_guid                   =   "B475F105-78FD-4200-96BC-148BAB6DAC11";
        string  value_s_ip                      =   "213.152.6.89";
        int     value_s_ip_changes              =   0;
        int     value_sc_count                  =   7;
        string  value_uri                       =   "http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8";

        dictRes["errorCode"]                    =   int64_t(errorCode);
        vector<boost::any> array(1);
        map<string, boost::any> valueDict;
        valueDict["bytes"]                      =   int64_t(value_bytes);
        valueDict["c-duration-downloaded"]      =   value_c_duration_downloaded;
        valueDict["c-duration-watched"]         =   value_c_duration_watched;
        valueDict["c-frames-dropped"]           =   int64_t(value_c_frames_dropped);
        valueDict["c-observed-bitrate"]         =   value_c_observed_bitrate;
        valueDict["c-overdue"]                  =   int64_t(value_c_overdue);
        valueDict["c-stalls"]                   =   int64_t(value_c_stalls);
        valueDict["c-start-time"]               =   value_c_start_time;
        valueDict["c-startup-time"]             =   value_c_startup_time;
        valueDict["cs-guid"]                    =   string(value_cs_guid);
        valueDict["s-ip"]                       =   string(value_s_ip);
        valueDict["s-ip-changes"]               =   int64_t(value_s_ip_changes);
        valueDict["sc-count"]                   =   int(value_sc_count);
        valueDict["uri"]                        =   string(value_uri);

        array[0]                                =   valueDict;
        valueDict["value"]                      =   array;
    }
    else if (!strcmp(argument, "playbackErrorLog"))
    {
        //get playback error log
    }

    http_response_add_header(res, "Content-Type", "application/x-apple-binary-plist");

    vector<char> msg;
    Plist::writePlistBinary(msg, dictRes);
    http_response_finish(res, reinterpret_cast<char *> (&msg[0]), msg.size());
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

// notify the server about the playback state.
void post_event_slideshow(const int &sock)
{
    string x_apple_session_ID   =   "f1634b51-5cae-4384-ade5-54f4159a15f1";
    string category             =   "slideshow";
    string lastAssetID          =   "5";
    string sessionID            =   "4";
    string state                =   "playing";

    http_request_t *req         =   http_request_init("HTTP/1.1", "POST", "/event");
    http_request_add_header(req, "Content-Type", "text/x-apple-plist+xml");
    http_request_add_header(req, "X-Apple-Session-ID", x_apple_session_ID.c_str());
    string msg_request          =   "";
    msg_request                 =   msg_request + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                                    + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                                    + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                                    + "<plist version=\"1.0\">" + "\n"
                                    + "<dict>" + "\n"
                                    + "<key>category</key>" + "\n"
                                    + "<string>" + category + "</string>" + "\n"
                                    + "<key>lastAssetID</key>" + "\n"
                                    + "<integer>" + lastAssetID + "</integer>" + "\n"
                                    + "<key>sessionID</key>" + "\n"
                                    + "<integer>" + sessionID + "</integer>" + "\n"
                                    + "<key>state</key>" + "\n"
                                    + "<string>" + state + "</string>" + "\n"
                                    + "</dict>" + "\n"
                                    + "</plist>" + "\n";
    const char *msg             =   msg_request.c_str();
    http_request_finish(req, msg, strlen(msg));
    send_req_to_socket(sock, req);
    http_request_destroy(req);

    http_parse_from_socket(sock);
}

//send the playback state to the client.
void post_event_video(const int &sock)
{
    string x_apple_session_ID   =   "f1634b51-5cae-4384-ade5-54f4159a15f1";
    string category             =   "video";
    string sessionID            =   "13";
    string state                =   "paused";

    http_request_t *req         =   http_request_init("HTTP/1.1", "POST", "/event");
    http_request_add_header(req, "Content-Type", "application/x-apple-plist");
    http_request_add_header(req, "X-Apple-Session-ID", x_apple_session_ID.c_str());
    string msg_request          =   "";
    msg_request                 =   msg_request + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                                    + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                                    + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                                    + "<plist version=\"1.0\">" + "\n"
                                    + "<dict>" + "\n"
                                    + "<key>category</key>" + "\n"
                                    + "<string>" + category + "</string>" + "\n"
                                    + "<key>sessionID</key>" + "\n"
                                    + "<integer>" + sessionID + "</integer>" + "\n"
                                    + "<key>state</key>" + "\n"
                                    + "<string>" + state + "</string>" + "\n"
                                    + "</dict>" + "\n"
                                    + "</plist>" + "\n";
    const char *msg             =   msg_request.c_str();
    http_request_finish(req, msg, strlen(msg));
    send_req_to_socket(sock, req);
    http_request_destroy(req);
    http_parse_from_socket(sock);
}

void post_reverse(const int &sock, const Request &rq)
{
    std::map<string, string> map  =   rq.get_headers_map();
    string x_apple_session_ID     =   map["X-Apple-Session-ID"];
    string x_apple_device_ID      =   map["X-Apple-Device-ID"];
    string x_apple_purpose        =   map["X-Apple-Purpose"];

    http_response_t *res    =   http_response_init("HTTP/1.1", 101, "Switching Protocols");
    http_response_add_header(res, "Upgrade", "PTTH/1.0");
    http_response_add_header(res, "Connection", "Upgrade");
    http_response_finish(res, NULL, 0);
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//Retrieve information about the server capabilities.
void get_stream(const int &sock)
{
    http_response_t *res    =   http_response_init("HTTP/1.1", 200, "OK");

    int     height          =   720;
    bool    overscanned     =   true;
    double  refreshRate     =   0.016666666666666666;
    string  version         =   "130.14";
    int     width           =   1280;

    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");

    map<string, boost::any> dictRes;
    dictRes["height"]       =   int64_t(height);
    dictRes["overscanned"]  =   overscanned;
    dictRes["refreshRate"]  =   refreshRate;
    dictRes["version"]      =   string(version);
    dictRes["width"]        =   int64_t(width);

    vector<char> msg;
    Plist::writePlistXML(msg, dictRes);
    http_response_finish(res, reinterpret_cast<char *> (&msg[0]), msg.size());
    send_res_to_socket(sock, res);
    http_response_destroy(res);
}

//The client sends a binary property list with information about the stream
void post_stream(const int &sock, const Request &rq)
{
    string deviceID;
    string latencyMs;
    vector<string> fpsInfo;
    vector<char> param1;
    vector<char> param2;
    string sessionID;
    string version;
    vector<string> timestampInfo;

    // FILE *fr = fopen("stream.plist", "wb");
    // write_to_file(fr, rq.get_data(), rq.get_len_data());

    map<string, boost::any> dict;
    Plist::readPlist(rq.get_data(), rq.get_len_data(), dict);

    //diffirent protocol

    //deviceID
    deviceID = boost::any_cast<const string &>(dict.find("deviceID")->second);
    print_debug("deviceID: %s\n", deviceID.c_str());

    //FpsInfo
    print_debug("FpsInfo: ");
    const vector<boost::any> &arrayFpsInfo = boost::any_cast<const vector<boost::any>&>(dict.find("fpsInfo")->second);
    for (unsigned int i = 0; i < arrayFpsInfo.size(); i++)
    {
        fpsInfo.push_back(boost::any_cast<const string &>(arrayFpsInfo[i]));
        print_debug("%s\n", fpsInfo[i].c_str());
    }

    //latencyMs
    latencyMs = boost::any_cast<const string &>(dict.find("latencyMs")->second);
    print_debug("latencyMs: %s\n", latencyMs.c_str());

    // //param1
    // param1 = boost::any_cast<const vector<char> &>(dict.find("param1")->second);

    // //param2
    // param2 = boost::any_cast<const vector<char> &>(dict.find("param2")->second);

    //sessionID
    sessionID = boost::any_cast<const string &>(dict.find("sessionId")->second);
    print_debug("sessionID: %s\n", sessionID.c_str());

    //timestampInfo
    print_debug("timestampInfo: ");
    const vector<boost::any> &arrayTimestampInfo = boost::any_cast<const vector<boost::any>&>(dict.find("timestampInfo")->second);
    for (unsigned int i = 0; i < arrayTimestampInfo.size(); i++)
    {
        timestampInfo.push_back(boost::any_cast<const string &>(arrayTimestampInfo[i]));
        print_debug("%s\n", timestampInfo[i].c_str());
    }

    //version
    version = boost::any_cast<const string &>(dict.find("version")->second);
    print_debug("version: %s\n", version.c_str());

    //read headers stream packets
    int             n                       = 0;
    unsigned int    length_loaded           = 0;
    const int       BUFFER_HEADER_SIZE      = 128;
    char            buffer_header[BUFFER_HEADER_SIZE];
    unsigned int    buffer_payload_size     = 150000;
    char            *buffer_payload         = new char[buffer_payload_size]();
    // int             length_file             = 0;
    //FILE        *video                  = fopen("stream.avi", "ab");

    while (1)
    {
        n = 0;
        length_loaded = 0;
        while (length_loaded < BUFFER_HEADER_SIZE)
        {
            n = read(sock, buffer_header + n, BUFFER_HEADER_SIZE - length_loaded);
            length_loaded += n;
            if (n < 0) error("ERROR reading header from socket");
            if (n == 0) exit(0);

            print_debug("n: %d\nbuffer_payload_size: %u\n", n, buffer_payload_size);
        }

        header_stream_packets header;
        memcpy((char *)&header, buffer_header, sizeof(header));

        unsigned short  payload_type = header.payload_type;
        unsigned int    payload_size = header.payload_size;

        print_debug("payload_size: %u\n", payload_size);
        print_debug("payload_type: %u\n", payload_type);

        if (payload_size > buffer_payload_size)
        {
            buffer_payload_size = payload_size * 2;
            delete[] buffer_payload;
            log("delete buffer_payload");
            buffer_payload = new char[buffer_payload_size]();
        }

        n = 0;
        length_loaded = 0;
        while (length_loaded < payload_size)
        {
            n = read(sock, buffer_payload, payload_size - length_loaded);
            length_loaded += n;
            if (n < 0) error("ERROR reading payload from socket");
            if (n == 0) exit(0);

            print_debug("payload_size: %u\n", payload_size);
            print_debug("loaded: %u\n", length_loaded);

            switch (payload_type)
            {
            case 0:
                print_debug("video bitstream\n");
                //video bitstream
                break;
            case 1:
                print_debug("codec data\n");
                //codec data
                codec_data codec;
                memcpy((char *)&codec, buffer_payload, sizeof(codec));
                break;
            case 2:
                print_debug("heartbeat\n");
                //heartbeat
                break;
            default:
                error("error payload type\n");
                break;
            }
        }
    }
}



void send_command_to_remote_control(const string &command)
{
    int sock_remote = net_search_socket_remote_control();

    if (sock_remote > 0)
    {
        string host             =   "starlight.local.";
        string active_remote    =   "1986535575";
        string url              =   "";
        url                     =   url + "/ctrl-int/1/" + command;

        http_request_t *req     =   http_request_init("HTTP/1.1", "GET", url.c_str());
        http_request_add_header(req, "Host", host.c_str());
        http_request_add_header(req, "Active-Remote", active_remote.c_str());

        http_request_finish(req, NULL, 0);
        send_req_to_socket(sock_remote, req);
        http_request_destroy(req);
    }

    http_parse_from_socket(sock_remote);
}