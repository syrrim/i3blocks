#include <stdio.h>

#define ROOT "/sys/class/backlight/intel_backlight/"

int main(int argc, char ** argv){
    FILE* fp;
    int max;

    fp = fopen(ROOT"max_brightness", "r");
    if(!fp)return 1;
    fscanf(fp, "%d", &max);
    fclose(fp);
    if(argc > 1){
        float set;
        sscanf(argv[1], "%g", &set);
        if(set > 100){
            set = 100;
        }else if(set<0){
            set = 0;
        }
        fp = fopen(ROOT"brightness", "w");
        fprintf(fp, "%d", (int)((set * max) / 100));
        fclose(fp);
    }
    fp = fopen(ROOT"brightness", "r");
    int val;
    fscanf(fp, "%d", &val);
    printf("%g", val * 100.0 / max);
    fclose(fp);
    

}
