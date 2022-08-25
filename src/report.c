/* This code runs the configuration screen */
#include "main.h"

/* Include pros api */
#include "pros/apix.h"

/* Use pal log */
#define LOG_LEVEL_FILE LOG_LEVEL_WARN
#include "pal/log.h"

static bool report_has_init = false;

/* Report buffer sizing */
#define REPORT_BUF_LINES 9
/* Actual printable 44, allowing 48 bytes (including null) */
#define REPORT_BUF_CHARS 44

/* Circular buffer for the report - extra 4 bytes pad are aligned, guaranteed null */
static char report_buf[REPORT_BUF_LINES][REPORT_BUF_CHARS+4] = {0};
static int report_buf_next = 0;

/* Global text label */
static lv_obj_t * report_stream;

/* Public report print buffer - note string is truncated at null or 44 printable chars */
void report_print(const char * str)
{
    /* Verify report has init */
    if(!report_has_init)
    {
        LOG_ERROR("REPORT: Tried to report before initialized");
        return;
    }

    LOG_INFO("REPORT: Got request to report %s",str);
    LOG_DEBUG("buf next is %d",report_buf_next);
    /* Copy incoming string into print buffer as-is at the next entry space */
    strncpy(&report_buf[report_buf_next][0],str,REPORT_BUF_CHARS);

    /* Increment next line and roll over */
    report_buf_next++;
    if(report_buf_next >= REPORT_BUF_LINES)
    {
        report_buf_next = 0;
    }

    /* Now concat each string with a newline */
    static char report_out[REPORT_BUF_LINES*(REPORT_BUF_CHARS+1)+1];
    report_out[0] = 0;
    for(int i = 0; i < REPORT_BUF_LINES; i++)
    {
        /* Get index into circular buf and wrap */
        int j = i + report_buf_next;
        if(j >= REPORT_BUF_LINES)
        {
            j -= REPORT_BUF_LINES;
        }

        LOG_DEBUG("Concat'ing buf entry %d, line %d",j,i);

        /* Concat this line in the entry into the out string */
        strcat(report_out,&report_buf[j][0]);
        strcat(report_out,"\n");
    }

    /* Remove the last char (the newline) */
    report_out[strlen(report_out)-1] = 0;

    /* Send to graphics layer */
    lv_label_set_text(report_stream,report_out);
}



/* Function to initialize the report page */
void report_draw(lv_obj_t * page)
{
    /* Create a title */
    lv_obj_t * label, * newpage, * button, * icon;
    label = lv_label_create(page,NULL);
    lv_label_set_text(label,"REPORT");
    lv_obj_align(label,0,LV_ALIGN_IN_TOP_MID,0,0);

    /* Create a page for the report data */
    newpage = lv_page_create(page,NULL);
    lv_obj_set_style(newpage,&style_page);
    lv_obj_align(newpage,0,LV_ALIGN_IN_TOP_LEFT,4,32);
    lv_obj_set_size(newpage,408,200);
    lv_page_set_scrl_layout(newpage,LV_LAYOUT_COL_L);

    /* Create a label on that page */
    report_stream = lv_label_create(newpage,NULL);
    lv_label_set_text(report_stream,"");
    report_has_init = true;
}

