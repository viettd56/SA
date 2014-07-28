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
        else if (str_starts_with(url.c_str(), "/getProperty"))
        {
            cout << "POST /getProperty\n";
            post_getProperty(res);
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
            put_setProperty(rq, res);
        }
    }

    int datalen;
    // http_response_finish(res, NULL, 0);
    const char *data_res = http_response_get_data(res, &datalen);

    send_to_socket(sock, data_res, datalen);
    http_response_destroy(res);
}

void get_slideshow_features(http_response_t *res)
{

}
void put_photo(const Request &rq, http_response_t *res)
{

}
void put_slideshow_session(const Request &rq, http_response_t *res)
{

    params_map.clear();
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
        string msg_reponse = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        msg_reponse = msg_reponse + "\n"
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
void post_event(const Request &rq, http_response_t *res)
{

}
void get_slideshow(const Request &rq, http_response_t *res)
{

}
void get_server_info(http_response_t *res)
{

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

    }
    http_response_finish(res, NULL, 0);
}
void post_scrub(const char *data, http_response_t *res)
{
    params_map.clear();
    //cout << data << "\n";
    attrs_map_str_parse(params_map, data);
    cout << params_map["position"] << "\n";
    http_response_finish(res, NULL, 0);
}
void post_rate(const char *data, http_response_t *res)
{
    params_map.clear();
    //cout << data << "\n";
    attrs_map_str_parse(params_map, data);
    cout << params_map["value"] << "\n";
    http_response_finish(res, NULL, 0);
}
void post_stop(http_response_t *res)
{

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

}
void put_setProperty(const Request &rq, http_response_t *res)
{

}
void post_getProperty(http_response_t *res)
{

}
void notify_event(const Request &rq, http_response_t *res)
{

}
void notify_slideshow(http_response_t *res)
{

}
void stop_photo_session(const Request &rq, http_response_t *res)
{

}

void post_reverse(http_response_t *res)
{

}