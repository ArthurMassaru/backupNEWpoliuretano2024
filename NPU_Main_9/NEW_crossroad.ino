//hyphotetical "better" crossroad

// void new_crossroad () {
//     if(elf >= BLACK || erf >= BLACK){
//         int flag2 = millis();
//         bool confirm_middle = 0, confirm_left = 0, confirm_right = 0;
//         while(millis() - flag2 <= 300){
//             array_read();
//             PID_crossroad(0.5);
//             if (elf >= BLACK) confirm_left = 1;
//             if (erf >= BLACK) confirm_right = 1;
//             if (mf >= BLACK) confirm_middle = 1;
//         }

//         if (confirm_left && confirm_middle & confirm_right) {
            
//         } else if (confirm_left && confirm_middle) {
//             int media_left, media_right, cross_cont, media_rl, media_gl, media_bl, media_rr, media_gr, media_br;
//             walk(700, 700);
//             while(elf<=BLACK){ //black baixo
//                 readLED();
//                 array_read();
//                 cross_cont++;
//                 media_left+=elf;
//                 media_right+=erf;
//                 media_rl+=rl;
//                 media_gl+=gl;
//                 media_bl+=bl;
//                 media_rr+=rr;
//                 media_gr+=gr;
//                 media_br+=br;
//                 delay(50);
//             }
//             walk(0, 0);

//             media_left/=cross_cont;
//             media_right/=cross_cont;
//             media_rl/=cross_cont;
//             media_gl/=cross_cont;
//             media_bl/=cross_cont;
//             media_rr/=cross_cont;
//             media_gr/=cross_cont;
//             media_br/=cross_cont;
            
//             bool comp_TCRT = media_left > media_right+30 ? 1 : 0;
        
//             bool comp_red = media_rl > media_rr+20 ? 1 : 0;
//             bool comp_green = media_gl > media_gr+20 ? 1 : 0;
//             bool comp_blue = media_bl > media_br+20 ? 1 : 0;

//             if((comp_red && comp_green && comp_blue) || (comp_red && comp_green && comp_TCRT) || (comp_red && comp_blue && comp_TCRT) || (comp_blue && comp_green && comp_TCRT)){
//                 walk(500, 500);
//                 delay(253);
//                 walk(0, 0);
//                 walk(-500, 500);
//                 delay(350);
//                 while (ls < BLACK)walk(800, -800);
//                 walk(0, 0);
//             }

//         } else if (confirm_left && confirm_right) {
//             while (ls < BLACK)walk(800, -800);
//             delay(300);
//             while (ls < BLACK)walk(800, -800);
//             walk(0, 0);

//         } else if (confirm_middle && confirm_right) {
            
//             int media_left, media_right, cross_cont, media_rl, media_gl, media_bl, media_rr, media_gr, media_br;
//             walk(700, 700);
//             while(elf<=BLACK){
//                 readLED();
//                 array_read();
//                 cross_cont++;
//                 media_left+=elf;
//                 media_right+=erf;
//                 media_rl+=rl;
//                 media_gl+=gl;
//                 media_bl+=bl;
//                 media_rr+=rr;
//                 media_gr+=gr;
//                 media_br+=br;
//                 delay(50);
//             }
//             walk(0, 0);

//             media_left/=cross_cont;
//             media_right/=cross_cont;
//             media_rl/=cross_cont;
//             media_gl/=cross_cont;
//             media_bl/=cross_cont;
//             media_rr/=cross_cont;
//             media_gr/=cross_cont;
//             media_br/=cross_cont;
            
//             bool comp_TCRT = media_left < media_right+30 ? 1 : 0;
        
//             bool comp_red = media_rl < media_rr+20 ? 1 : 0;
//             bool comp_green = media_gl < media_gr+20 ? 1 : 0;
//             bool comp_blue = media_bl < media_br+20 ? 1 : 0;

//             if((comp_red && comp_green && comp_blue) || (comp_red && comp_green && comp_TCRT) || (comp_red && comp_blue && comp_TCRT) || (comp_blue && comp_green && comp_TCRT)){
//                 walk(500, 500);
//                 delay(253);
//                 walk(0, 0);
//                 walk(500, -500);
//                 delay(350);
//                 while (rs < BLACK)walk(-800, 800);
//                 walk(0, 0);
//             }

//         } else if (confirm_left) {
//             while (ms > WHITE) {
//                 array_read();
//                 walk(800, 800);               
//             }
//             while (rs < BLACK)walk(-800, 800);
//         } else if (confirm_right) {
//             while (ms > WHITE) {
//                 array_read();
//                 walk(800, 800);               
//             }
//             while (ls < BLACK)walk(800, -800);
//         }
//     }
// }
