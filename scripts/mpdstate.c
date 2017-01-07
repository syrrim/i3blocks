// vim:ts=4:sw=4:expandtab
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <yajl/yajl_gen.h>
#include <yajl/yajl_version.h>

#include <mpd/status.h>
#include <mpd/song.h>
#include <mpd/connection.h>

#include "i3status.h"

//https://www.reddit.com/r/linux/comments/2399hw/mpdmpc_any_way_to_get_current_time_of_song_in/cgva2tg/
//use what works, right?
int main(void)
{
    unsigned elapsed = 0;
    unsigned duration = 0;
    struct mpd_connection *conn;
    struct mpd_status *status;
    enum mpd_state state;

    conn = mpd_connection_new(NULL, 0, 0);

    status = mpd_run_status(conn);
    if (!status) return 1;
    elapsed = mpd_status_get_elapsed_time(status);
    duration = mpd_status_get_total_time(status);
    state = mpd_status_get_state(status);
    mpd_status_free(status);

    mpd_connection_free(conn);

    if(state == MPD_STATE_STOP){
	printf("stopped\n");
    }else if(state == MPD_STATE_PLAY){
    	printf("playing\n");
    }else if(state == MPD_STATE_PAUSE){
    	printf("paused\n");
    }else{
    	printf("unknown\n");
    }
    printf("%u:%02u/%u:%02u\n", elapsed/60, elapsed%60, duration/60, duration%60);

    return 0;
}
