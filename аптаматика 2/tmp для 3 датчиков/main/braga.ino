
void build_braga() {
  BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.UPDATE("INDICATOR_1,T_1,T_2,T_1", 3000);
  GP_MAKE_BOX(GP.TITLE("Брожение by Апта"); GP.COLOR("INDICATOR_1", valCol, true););
  GP.HR(); 
  GP.NAV_TABS("Общая температура,Температура термометра №1,Температура термометра №2,Температура термометра №3");

  
  GP.NAV_BLOCK_BEGIN();
  GP.FORM_BEGIN("/selected_T");
  GP_MAKE_BLOCK(
        GP.LABEL("Термометр №1");
        GP.SWITCH("S_T_1", S_T_1);
    );
  GP_MAKE_BLOCK(
        GP.LABEL("Термометр №2");
        GP.SWITCH("S_T_2", S_T_2);
    );
  GP_MAKE_BLOCK(
        GP.LABEL("Термометр №3");
        GP.SWITCH("S_T_3", S_T_3);
    );
  GP.SUBMIT("Сохранить");
  GP.FORM_END();
  
  GP.TITLE("Термометр №1");
  GP.LABEL(String(T_cube), "T_1");

  GP.TITLE("Термометр №2");
  GP.LABEL(String(T_col_mid), "T_2");

  GP.TITLE("Термометр №3");
  GP.LABEL(String(T_jets), "T_3");
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.TITLE("Термометр №1");
  GP.LABEL(String(T_cube), "T_1_");
  GP.PLOT_DARK<1, PLOT_CUBE_SIZE>("plot_1", names_cube_B, arr_T_cube, 100);
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.TITLE("Термометр №2");
  GP.LABEL(String(T_col_mid), "T_2_");
  GP.PLOT_DARK<1, PLOT_DIF_SIZE>("plot_2", names_dif_B, arr_T_dif_B, 100);
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.TITLE("Термометр №3");
  GP.LABEL(String(T_jets), "T_3_");
  GP.PLOT_DARK<1, PLOT_JETS_SIZE>("plot_3", names_jets_B, arr_T_jets, 100);
  GP.NAV_BLOCK_END();
  
  BUILD_END();
}


void action_braga() {
  if (portal.form("/selected_T")) {      // кнопка нажата
    S_T_1 = portal.getBool("S_T_1");
    S_T_2 = portal.getBool("S_T_2");
    S_T_3 = portal.getBool("S_T_3");
  }
  
  if (portal.update()) {

    portal.updateColor("INDICATOR_1", valCol);
    
    if (S_T_1){
      portal.updateFloat("T_1", T_cube);
      portal.updateFloat("T_1_", T_cube);
    }

    if (S_T_2){
      portal.updateFloat("T_2", T_col_mid);
      portal.updateFloat("T_2_", T_col_mid);
    }

    if (S_T_3){
      portal.updateFloat("T_3", T_jets);
      portal.updateFloat("T_3_", T_jets);
    }
    
  }
}


void braga_loop(){
  
  if (S_T_1 || S_T_2 || S_T_3){
    static bool flag_crit_temp = false;
    static bool flag_bad_temp = false;
    read_braga_temp();
    
    if (S_T_1 && T_cube != 0){
      if (!flag_crit_temp && (T_cube < 17 || T_cube > 36)) {
        flag_crit_temp = true;
        bot.sendMessage("Температура браги 1 критическая: " + String(T_cube));
        valCol.setHEX(0xff0000);
      }
      else if (!flag_bad_temp && (T_cube < 22 || T_cube > 32)) {
        flag_bad_temp = true;
        bot.sendMessage("Температура браги 1 слегка не под контролем: " + String(T_cube));
        valCol.setHEX(0x0000ff);
      }
      else if (T_cube > 22 && T_cube < 32) {
        flag_crit_temp = false;
        flag_bad_temp = false;
        valCol.setHEX(0x00ff00);
      }
    }
  }
}
