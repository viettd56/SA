#include "callback.hpp"
#include <iostream>
#include "util.hpp"
#include "pugixml.hpp"
#include <cstring>
#include <map>
#include <string>

using std::cout;

std::map<string, string> params_map;
enum state_param {KEY, VALUE};
state_param state_params = KEY;
string last_key = "";

void callback_excute(const int &sock, const Request &rq)
{
    http_response_t *res = NULL;
    // headers_map = get_headers_map();
    // params_map = get_params_map();
    string method = rq.get_method();
    string url = rq.get_url();
    // cout << method << "\n" << url << "\n";
    // char *data = rq.get_data();

    res = http_response_init("HTTP/1.1", 200, "OK");

    if (!method.compare("GET"))
    {
        if (!url.compare("/slideshow-features"))
        {
            cout << "GET /slideshow-features\n";
            get_slideshow_features(res);
        }
        else if (!url.compare("/server-info"))
        {
            cout << "GET  /server-info\n";
            get_server_info(res);

        }
        else if (!url.compare("/scrub"))
        {
            cout << "GET /scrub\n";
            get_scrub(res);
        }
        else if (!url.compare("/playback-info"))
        {
            cout << "GET /playback-info\n";
            get_playback_info(res);
        }
        else if (str_starts_with(url.c_str(), "/getProperty"))
        {
            cout << "GET /getProperty\n";
            get_getProperty(url.c_str() + strlen("/getProperty?"), res);
        }

    }
    else if (!method.compare("POST"))
    {
        if (!url.compare("/stop"))
        {
            cout << "POST /stop\n";
            post_stop(res);
        }
        else if (!url.compare("/reverse"))
        {
            cout << "POST /reverse\n";
            post_reverse(res);
        }
        else if (!url.compare("/play"))
        {
            cout << "POST /play\n";
            post_play(rq, res);
        }
        else if (str_starts_with(url.c_str(), "/scrub"))
        {
            cout << "POST /scrub\n";
            post_scrub(url.c_str() + strlen("/scrub?"), res);
        }
        else if (str_starts_with(url.c_str(), "/rate"))
        {
            cout << "POST /rate\n";
            post_rate(url.c_str() + strlen("/rate?"), res);
        }
    }
    else if (!method.compare("PUT"))
    {
        if (!url.compare("/photo"))
        {
            cout << "PUT /photo\n";
            put_photo(rq, res);
        }
        else if (!url.compare("/slideshows/1"))
        {
            cout << "PUT /slideshows/1\n";
            put_slideshow_session(rq, res);
        }
        else if (str_starts_with(url.c_str(), "/setProperty"))
        {
            cout << "PUT /setProperty\n";
            put_setProperty(url.c_str() + strlen("/setProperty?"), rq, res);
        }
    }

    int datalen;
    // http_response_finish(res, NULL, 0);
    const char *data_res = http_response_get_data(res, &datalen);

    send_to_socket(sock, data_res, datalen);
    http_response_destroy(res);

    //------------------------------------------------

    // http_request_t *req = post_event_slideshow();
    // int datalen;
    // const char *data_res = http_request_get_data(req, &datalen);

    // send_to_socket(sock, data_res, datalen);
    // http_request_destroy(req);
}

void get_slideshow_features(http_response_t *res)
{
    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
    string key = "Reflections";
    string name = "Reflections";

    string msg_reponse = "";
    msg_reponse = msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
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
    const char *msg = msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
}
void put_photo(const Request &rq, http_response_t *res)
{
    std::map<string, string> map = rq.get_headers_map();
    string x_apple_assetKey = map["X-Apple-AssetKey"];
    string x_apple_transition = map["X-Apple-Transition"];
    string x_apple_assetAction = map["X-Apple-AssetAction"];
    if (x_apple_transition == "Dissolve")
    {
        //if picture using the dissolve transition
        cout << "Picture using the dissolve transition" << "\n";
    }
    else
    {
        //if show a picture without any transition
        cout << "Show a picture without any transition" << "\n";
    }

    if (x_apple_assetAction == "cacheOnly")
    {
        //cache a picture for future display
        cout << "cache a picture for future display" << "\n";
    }
    else if (x_apple_assetAction == "displayCached")
    {
        //show a cached picture
        cout << "show a cached picture" << "\n";
    }

    http_response_finish(res, NULL, 0);
}
void put_slideshow_session(const Request &rq, http_response_t *res)
{

    params_map.clear();
    state_params = KEY;
    //std::cout << "DATA:" << rq.get_data() << "\n";
    pugi::xml_document doc;

    {
        pugi::xml_parse_result result = doc.load_buffer(rq.get_data(), rq.get_len_data());
        pugi::xpath_node first_key = doc.select_single_node("plist/dict/key");
        for (pugi::xml_node node = first_key.node(); node; node = node.next_sibling())
        {

            if (!strcmp(node.child_value(), "settings"))
            {
                node = node.next_sibling();
                for (pugi::xml_node node_settings = node.first_child(); node_settings; node_settings = node_settings.next_sibling())
                {

                    if (strcmp(node_settings.child_value(), ""))
                    {
                        // std::cout << "Settings: " << node_settings.child_value() << "\n";
                        if (state_params == KEY)
                        {
                            last_key = node_settings.child_value();
                            state_params = VALUE;
                        }
                        else
                        {
                            params_map[last_key] = node_settings.child_value();
                            state_params = KEY;
                        }
                    }
                }
            }

            if (strcmp(node.child_value(), ""))
            {
                // std::cout << "Value: " << node.child_value() << "\n";
                if (state_params == KEY)
                {
                    last_key = node.child_value();
                    state_params = VALUE;
                }
                else
                {
                    params_map[last_key] = node.child_value();
                    state_params = KEY;
                }
            }
        }

        http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
        // cout << params_map["theme"] << "\n";
        string msg_reponse = "";
        msg_reponse = msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                      + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                      + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                      + "<plist version=\"1.0\">" + "\n"
                      + "<dict/>" + "\n"
                      + "</plist>" + "\n";
        const char *msg = msg_reponse.c_str();
        http_response_finish(res, msg, strlen(msg));
        //cout << "res:" << res << "\n";

    }
}
void fetch_photo(http_response_t *res)
{

}
void post_stop_photo_slideshow(const Request &rq, http_response_t *res)
{

}
http_request_t *post_event_photo()
{
    string x_apple_session_ID = "1bd6ceeb-fffd-456c-a09c-996053a7a08c";
    string category = "photo";
    string sessionID = "38";
    string state = "stopped";

    http_request_t *req = http_request_init("HTTP/1.1", "POST", "/event");
    http_request_add_header(req, "Content-Type", "text/x-apple-plist+xml");
    http_request_add_header(req, "X-Apple-Session-ID", x_apple_session_ID.c_str());
    string msg_request = "";
    msg_request = msg_request + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
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
    const char *msg = msg_request.c_str();
    http_request_finish(req, msg, strlen(msg));
    return req;
}
void get_slideshow(const Request &rq, http_response_t *res)
{

}
void get_server_info(http_response_t *res)
{
    string deviceid = "58:55:CA:1A:E2:88";
    string features = "14839";
    string model = "AppleTV2,1";
    string protovers = "1.0";
    string srcvers = "120.2";

    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
    // cout << params_map["theme"] << "\n";
    string msg_reponse = "";
    msg_reponse = msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
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
    const char *msg = msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
}
void post_play(const Request &rq, http_response_t *res)
{
    if (rq.get_headers_map()["Content-Type"] == "text/parameters")
    {
        cout << rq.get_data() << "\n";
        params_map.clear();
        msgs_map_str_parse(params_map, rq.get_data());
        cout << params_map["Start-Position"] << "\n";
    }
    else if (rq.get_headers_map()["Content-Type"] == "application/x-apple-binary-plist")
    {
        //video playback from iPhone
        char *data = rq.get_data();
        char *binary_data = strsep(&data, "\r\n\r\n"); // BINARY PLIST DATA
        char *xml = data;

        params_map.clear();
        state_params = KEY;
        //std::cout << "DATA:" << rq.get_data() << "\n";
        pugi::xml_document doc;

        {
            pugi::xml_parse_result result = doc.load_buffer(xml, strlen(xml));
            pugi::xpath_node first_key = doc.select_single_node("plist/dict/key");
            for (pugi::xml_node node = first_key.node(); node; node = node.next_sibling())
            {

                if (strcmp(node.child_value(), ""))
                {
                    // std::cout << "Value: " << node.child_value() << "\n";
                    if (state_params == KEY)
                    {
                        last_key = node.child_value();
                        state_params = VALUE;
                    }
                    else
                    {
                        params_map[last_key] = node.child_value();
                        state_params = KEY;
                    }
                }
            }
        }
    }
    http_response_finish(res, NULL, 0);
}
void post_scrub(const char *argument, http_response_t *res)
{
    params_map.clear();
    //cout << data << "\n";
    attrs_map_str_parse(params_map, argument);
    cout << params_map["position"] << "\n";
    http_response_finish(res, NULL, 0);
}
void post_rate(const char *argument, http_response_t *res)
{
    params_map.clear();
    //cout << data << "\n";
    attrs_map_str_parse(params_map, argument);
    cout << params_map["value"] << "\n";
    http_response_finish(res, NULL, 0);
}
void post_stop(http_response_t *res)
{
    http_response_finish(res, NULL, 0);
}
void get_scrub(http_response_t *res)
{
    http_response_add_header(res, "Content-Type", "text/parameters");
    string duration = doubletostr(83.124794);
    string position = doubletostr(14.467000);

    string msg_reponse = "";
    msg_reponse = msg_reponse + "duration: " + duration + "\nposition: " + position;
    cout << msg_reponse << "\n";
    const char *msg = msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));

}
void get_playback_info(http_response_t *res)
{
    string duration = "1801";
    string loadedTimeRanges_duration = "51.541130402";
    string loadedTimeRanges_start = "18.118717650000001";
    string playbackBufferEmpty = "true";
    string playbackBufferFull = "false";
    string playbackLikelyToKeepUp = "true";
    string position = "18.043869775000001";
    string rate = "1";
    string readyToPlay = "true";
    string seekableTimeRanges_duration = "1801";
    string seekableTimeRanges_start = "0.0";

    http_response_add_header(res, "Content-Type", "text/x-apple-plist+xml");
    // cout << params_map["theme"] << "\n";
    string msg_reponse = "";
    msg_reponse = msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
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
    const char *msg = msg_reponse.c_str();
    http_response_finish(res, msg, strlen(msg));
}
void put_setProperty(const char *argument, const Request &rq, http_response_t *res)
{
    char *data = rq.get_data();
    char *binary_data = strsep(&data, "\r\n\r\n"); // BINARY PLIST DATA
    char *xml = data;

    params_map.clear();
    state_params = KEY;
    //std::cout << "DATA:" << rq.get_data() << "\n";
    pugi::xml_document doc;

    {
        pugi::xml_parse_result result = doc.load_buffer(xml, strlen(xml));
        pugi::xpath_node first_key = doc.select_single_node("plist/dict/key");
        for (pugi::xml_node node = first_key.node(); node; node = node.next_sibling())
        {

            if (!strcmp(node.child_value(), "value"))
            {
                node = node.next_sibling();
                for (pugi::xml_node node_settings = node.first_child(); node_settings; node_settings = node_settings.next_sibling())
                {

                    if (strcmp(node_settings.child_value(), ""))
                    {
                        // std::cout << "Settings: " << node_settings.child_value() << "\n";
                        if (state_params == KEY)
                        {
                            last_key = node_settings.child_value();
                            state_params = VALUE;
                        }
                        else
                        {
                            params_map[last_key] = node_settings.child_value();
                            state_params = KEY;
                        }
                    }
                }
            }

            if (strcmp(node.child_value(), ""))
            {
                // std::cout << "Value: " << node.child_value() << "\n";
                if (state_params == KEY)
                {
                    last_key = node.child_value();
                    state_params = VALUE;
                }
                else
                {
                    params_map[last_key] = node.child_value();
                    state_params = KEY;
                }
            }
        }
    }

    string errorCode = "0";
    char *binary_data_reponse = NULL;

    if (!strcmp(argument, "forwardEndTime"))
    {
        //set forward end time
    }
    else if (!strcmp(argument, "reverseEndTime"))
    {
        //set reverse end time
    }

    http_response_add_header(res, "Content-Type", "application/x-apple-binary-plist");
    string msg_reponse = "\r\n";
    msg_reponse = msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                  + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                  + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                  + "<plist version=\"1.0\">" + "\n"
                  + "<dict>" + "\n"
                  + "<key>errorCode</key>" + "\n"
                  + "<integer>" + errorCode + "</integer>" + "\n"
                  + "</dict>" + "\n"
                  + "</plist>" + "\n";
    const char *msg = str_concat(binary_data_reponse, msg_reponse.c_str());
    http_response_finish(res, msg, strlen(msg));
    delete[] msg;
    //delete binary_data_reponse
}

void get_getProperty(const char *argument, http_response_t *res)
{
    string errorCode = "0";
    string value_bytes = "1818336";
    string value_c_duration_downloaded = "70";
    string value_c_duration_watched = "18.154102027416229";
    string value_c_frames_dropped = "0";
    string value_c_observed_bitrate = "14598047.302367469";
    string value_c_overdue = "0";
    string value_c_stalls = "0";
    string value_c_start_time = "0.0";
    string value_c_startup_time = "0.27732497453689575";
    string value_cs_guid = "B475F105-78FD-4200-96BC-148BAB6DAC11";
    string value_date = "2012-03-16T15:31:24Z";
    string value_s_ip = "213.152.6.89";
    string value_s_ip_changes = "0";
    string value_sc_count = "7";
    string value_uri = "http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8";
    char *binary_data_reponse = NULL;

    if (!strcmp(argument, "playbackAccessLog"))
    {
        //set forward end time
    }
    else if (!strcmp(argument, "playbackErrorLog"))
    {
        //set reverse end time
    }

    http_response_add_header(res, "Content-Type", "application/x-apple-binary-plist");
    string msg_reponse = "\r\n";
    msg_reponse = msg_reponse + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
                  + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" + "\n"
                  + "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + "\n"
                  + "<plist version=\"1.0\">" + "\n"
                  + "<dict>" + "\n"
                  + "<key>errorCode</key>" + "\n"
                  + "<integer>" + errorCode + "</integer>" + "\n"
                  + "<key>value</key>" + "\n"
                  + "<array>" + "\n"
                  + "<dict>" + "\n"
                  + "<key>bytes</key> <integer>" + value_bytes + "</integer>" + "\n"
                  + "<key>c-duration-downloaded</key> <real>" + value_c_duration_downloaded + "</real>" + "\n"
                  + "<key>c-duration-watched</key> <real>" + value_c_duration_watched + "</real>" + "\n"
                  + "<key>c-frames-dropped</key> <integer>" + value_c_frames_dropped + "</integer>" + "\n"
                  + "<key>c-observed-bitrate</key> <real>" + value_c_observed_bitrate + "</real>" + "\n"
                  + "<key>c-overdue</key> <integer>" + value_c_overdue + "</integer>" + "\n"
                  + "<key>c-stalls</key> <integer>" + value_c_stalls + "</integer>" + "\n"
                  + "<key>c-start-time</key> <real>" + value_c_start_time + "</real>" + "\n"
                  + "<key>c-startup-time</key> <real>" + value_c_startup_time + "</real>" + "\n"
                  + "<key>cs-guid</key> <string>" + value_cs_guid + "</string>" + "\n"
                  + "<key>date</key> <date>" + value_date + "</date>" + "\n"
                  + "<key>s-ip</key> <string>" + value_s_ip + "</string>" + "\n"
                  + "<key>s-ip-changes</key> <integer>" + value_s_ip_changes + "</integer>" + "\n"
                  + "<key>sc-count</key> <integer>" + value_sc_count + "</integer>" + "\n"
                  + "<key>uri</key> <string>" + value_uri + "</string>" + "\n"
                  + "</dict>" + "\n"
                  + "</array>" + "\n"
                  + "</dict>" + "\n"
                  + "</plist>" + "\n";
    const char *msg = str_concat(binary_data_reponse, msg_reponse.c_str());
    http_response_finish(res, msg, strlen(msg));
    delete[] msg;
    //delete binary_data_reponse
}
void notify_event(const Request &rq, http_response_t *res)
{

}
http_request_t *post_event_slideshow()
{
    string x_apple_session_ID = "f1634b51-5cae-4384-ade5-54f4159a15f1";
    string category = "slideshow";
    string lastAssetID = "5";
    string sessionID = "4";
    string state = "playing";

    http_request_t *req = http_request_init("HTTP/1.1", "POST", "/event");
    http_request_add_header(req, "Content-Type", "text/x-apple-plist+xml");
    http_request_add_header(req, "X-Apple-Session-ID", x_apple_session_ID.c_str());
    string msg_request = "";
    msg_request = msg_request + "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n"
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
    const char *msg = msg_request.c_str();
    http_request_finish(req, msg, strlen(msg));
    return req;
}
void stop_photo_session(const Request &rq, http_response_t *res)
{

}

void post_reverse(http_response_t *res)
{

}