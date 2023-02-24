#pragma once
//--------------------------------------------------------------------------
// FilePass：画像ファイルのパスを格納するヘッダです。
//--------------------------------------------------------------------------
// [名前付けルール（暫定）]
// ・全て大文字、アンダーバーで表記する
// ・画像ファイルはGH（Graphic Handleの略）、音声ファイルはSH（Sound Handleの略）を先頭に付ける
// （Visual StudioのIntelliCodeの検索結果に引っかかりやすくするため）
// ・文字はLOGO、背景はBG（backgroundの略）、武器はICONのようにカテゴリ分けする
// ・
// [備考]
// ・特に理由がない限り画像ファイルはPNG形式、音声ファイルはOGG形式にしておきます
//--------------------------------------------------------------------------

//【画像ファイル】

#define GH_LOGO_TITLE_BO ("resource/img/logo/titleLogo_bo.png")//タイトルロゴ（黒字橙枠）
#define GH_LOGO_TITLE_BW ("resource/img/logo/titleLogo_bw.png")//タイトルロゴ（黒字白枠）
#define GH_LOGO_TITLE_SECTION_1_BO ("resource/img/logo/section1_bo.png")//タイトル項目１（黒字橙枠）
#define GH_LOGO_TITLE_SECTION_1_BW ("resource/img/logo/section1_bw.png")//タイトル項目１（黒字白枠）
#define GH_LOGO_TITLE_SECTION_2_BO ("resource/img/logo/section2_bo.png")//タイトル項目２（黒字橙枠）
#define GH_LOGO_TITLE_SECTION_2_BW ("resource/img/logo/section2_bw.png")//タイトル項目２（黒字白枠）
#define GH_LOGO_TITLE_SECTION_3_BO ("resource/img/logo/section3_bo.png")//タイトル項目３（黒字橙枠）
#define GH_LOGO_TITLE_SECTION_3_BW ("resource/img/logo/section3_bw.png")//タイトル項目３（黒字白枠）
#define GH_LOGO_DATABASE_BO ("resource/img/logo/database_bo.png")//データベースロゴ（黒字橙枠）
#define GH_LOGO_DATABASE_BW ("resource/img/logo/database_bw.png")//データベースロゴ（黒字白枠）
#define GH_LOGO_TITLE_DATABASE_1_BO ("resource/img/logo/weaponData_bo.png")//データベース項目１（黒字橙枠）
#define GH_LOGO_TITLE_DATABASE_1_BW ("resource/img/logo/weaponData_bw.png")//データベース項目１（黒字白枠）
#define GH_LOGO_TITLE_DATABASE_2_BO ("resource/img/logo/performData_bo.png")//データベース項目２（黒字橙枠）
#define GH_LOGO_TITLE_DATABASE_2_BW ("resource/img/logo/performData_bw.png")//データベース項目２（黒字白枠）
#define GH_LOGO_SELECT_BO ("resource/img/logo/select_bo.png")//ステージセレクト（黒字橙枠）
#define GH_LOGO_SELECT_BW ("resource/img/logo/select_bw.png")//ステージセレクト（黒字白枠）
#define GH_LOGO_SCORE_BW ("resource/img/logo/score.png")//得点（黒字白枠）
#define GH_LOGO_SEC_BW ("resource/img/logo/sec.png")//秒数（黒字白枠）
#define GH_LOGO_TIME_BW ("resource/img/logo/time.png")//残り時間（黒字白枠）
#define GH_LOGO_NUMBER_BW ("resource/img/logo/num.png")//数字（黒字白枠）
#define GH_LOGO_RANK_W ("resource/img/logo/rank.png")//スコアランク
#define GH_LOGO_ABORT_BO ("resource/img/logo/Abortgame_bo.png")//中止
#define GH_LOGO_ABORT_BW ("resource/img/logo/Abortgame_bw.png")//中止

#define GH_MIMI_ISE ("resource/img/char/ise.png")//耳（伊勢）
#define GH_MIMI_IMIYA ("resource/img/char/imiya.png")//耳（井宮）
#define GH_MIMI_URAYAMA ("resource/img/char/urayama.png")//耳（浦山）
#define GH_MIMI_KAKITA ("resource/img/char/kakita.png")//耳（柿田）
#define GH_MIMI_KAZIWARA ("resource/img/char/kaziwara.png")//耳（梶原）

#define GH_ICON_COTTON ("resource/img/icon/i_menbo.png")//綿棒（アイコン）
#define GH_ICON_PICK ("resource/img/icon/i_mimikaki.png")//耳かき（アイコン）
#define GH_ICON_WRIST_CUTTER_G ("resource/img/icon/i_wcg.png")//リストカッターG（アイコン）
#define GH_ICON_DEEPKISS ("resource/img/icon/i_bera.png")//ベラ噛み（アイコン）
#define GH_ICON_OMORI ("resource/img/icon/i_omori.png")//大森アックスボンバー（アイコン）
#define GH_ICON_NODON ("resource/img/icon/i_nodon.png")//ノドン（アイコン）

#define GH_FLAME_COTTON ("resource/img/icon/flame/f_menbo.png")//綿棒（フレーム）
#define GH_FLAME_PICK ("resource/img/icon/flame/f_mimikaki.png")//耳かき（フレーム）
#define GH_FLAME_WRIST_CUTTER_G ("resource/img/icon/flame/f_wcg.png")//リストカッターG（フレーム）
#define GH_FLAME_DEEPKISS ("resource/img/icon/flame/f_bera.png")//ベラ噛み（フレーム）
#define GH_FLAME_OMORI ("resource/img/icon/flame/f_omori.png")//大森アックスボンバー（フレーム）
#define GH_FLAME_NODON ("resource/img/icon/flame/f_nodon.png")//ノドン（フレーム）

#define GH_ATLAS_COTTON ("resource/img/icon/flame/atlas/a_menbo.png")//綿棒（アトラス）
#define GH_ATLAS_PICK ("resource/img/icon/flame/atlas/a_mimikaki.png")//耳かき（アトラス）
#define GH_ATLAS_WRIST_CUTTER_G ("resource/img/icon/flame/atlas/a_wcg.png")//リストカッターG（アトラス）
#define GH_ATLAS_DEEPKISS ("resource/img/icon/flame/atlas/a_bera.png")//ベラ噛み（アトラス）
#define GH_ATLAS_OMORI ("resource/img/icon/flame/atlas/a_omori.png")//大森アックスボンバー（アトラス）
#define GH_ATLAS_NODON ("resource/img/icon/flame/atlas/a_nodon.png")//ノドン（アトラス）

#define GH_BG_TITLE ("resource/img/bg/title.png")//背景（TitleScene）
#define GH_BG_SELECT ("resource/img/bg/select.png")//背景（SelectScene）
#define GH_BG_GAME ("resource/img/bg/game.png")//背景（GameScene）
#define GH_BG_RESULT ("resource/img/bg/result.png")//背景（ResultScene）
#define GH_BG_DATABESE ("resource/img/bg/database.png")//背景（DarabeseScene）

#define GH_GUIDE_1 ("resource/img/guide/guide_1.png")//ルール説明１
#define GH_GUIDE_2 ("resource/img/guide/guide_2.png")//ルール説明２
#define GH_GUIDE_3 ("resource/img/guide/guide_3.png")//ルール説明３
#define GH_GUIDE_4 ("resource/img/guide/guide_4.png")//ルール説明４
#define GH_GUIDE_5 ("resource/img/guide/guide_5.png")//ルール説明５

#define GH_DATA_COTTON ("resource/img/data/w_menbo.png")//綿棒（武器説明）
#define GH_DATA_PICK ("resource/img/data/w_mimikaki.png")//耳かき（武器説明）
#define GH_DATA_WRIST_CUTTER_G ("resource/img/data/w_wcg.png")//リストカッターG（武器説明）
#define GH_DATA_DEEPKISS ("resource/img/data/w_bera.png")//ベラ噛み（武器説明）
#define GH_DATA_OMORI ("resource/img/data/w_omori.png")//大森アックスボンバー（武器説明）
#define GH_DATA_NODON ("resource/img/data/w_nodon.png")//ノドン（武器説明）

#define GH_DATA_CLICK ("resource/img/data/p_click.png")//クリック（実績説明）
#define GH_DATA_EAR ("resource/img/data/p_ear.png")//耳（実績説明）
#define GH_DATA_WEAPON ("resource/img/data/p_weapon.png")//武器（実績説明）
#define GH_DATA_SCORE ("resource/img/data/p_score.png")//スコア（実績説明）
#define GH_DATA_PERFORM ("resource/img/data/p_perform.png")//実績（実績説明）

#define GH_DATA_CLICK_1_1 ("resource/img/data/p_click_1_1.png")//クリック1-100（実績説明）
#define GH_DATA_CLICK_1_2 ("resource/img/data/p_click_1_2.png")//クリック1-1000（実績説明）
#define GH_DATA_CLICK_1_3 ("resource/img/data/p_click_1_3.png")//クリック1-10000（実績説明）
#define GH_DATA_CLICK_1_4 ("resource/img/data/p_click_1_4.png")//クリック1-100000（実績説明）
#define GH_DATA_CLICK_2_1 ("resource/img/data/p_click_2_1.png")//クリック2-100（実績説明）
#define GH_DATA_CLICK_2_2 ("resource/img/data/p_click_2_2.png")//クリック2-1000（実績説明）
#define GH_DATA_CLICK_2_3 ("resource/img/data/p_click_2_3.png")//クリック2-10000（実績説明）
#define GH_DATA_CLICK_2_4 ("resource/img/data/p_click_2_4.png")//クリック2-100000（実績説明）
#define GH_DATA_CLICK_3_1 ("resource/img/data/p_click_3_1.png")//クリック3-100（実績説明）
#define GH_DATA_CLICK_3_2 ("resource/img/data/p_click_3_2.png")//クリック3-1000（実績説明）
#define GH_DATA_CLICK_3_3 ("resource/img/data/p_click_3_3.png")//クリック3-10000（実績説明）
#define GH_DATA_CLICK_3_4 ("resource/img/data/p_click_3_4.png")//クリック3-100000（実績説明）
#define GH_DATA_CLICK_4_1 ("resource/img/data/p_click_4_1.png")//クリック4-100（実績説明）
#define GH_DATA_CLICK_4_2 ("resource/img/data/p_click_4_2.png")//クリック4-1000（実績説明）
#define GH_DATA_CLICK_4_3 ("resource/img/data/p_click_4_3.png")//クリック4-10000（実績説明）
#define GH_DATA_CLICK_4_4 ("resource/img/data/p_click_4_4.png")//クリック4-100000（実績説明）
#define GH_DATA_CLICK_5_1 ("resource/img/data/p_click_5_1.png")//クリック5-100（実績説明）
#define GH_DATA_CLICK_5_2 ("resource/img/data/p_click_5_2.png")//クリック5-1000（実績説明）
#define GH_DATA_CLICK_5_3 ("resource/img/data/p_click_5_3.png")//クリック5-10000（実績説明）
#define GH_DATA_CLICK_5_4 ("resource/img/data/p_click_5_4.png")//クリック5-100000（実績説明）
#define GH_DATA_CLICK_6_1 ("resource/img/data/p_click_6_1.png")//クリック6-100（実績説明）
#define GH_DATA_CLICK_6_2 ("resource/img/data/p_click_6_2.png")//クリック6-1000（実績説明）
#define GH_DATA_CLICK_6_3 ("resource/img/data/p_click_6_3.png")//クリック6-10000（実績説明）
#define GH_DATA_CLICK_6_4 ("resource/img/data/p_click_6_4.png")//クリック6-100000（実績説明）
#define GH_DATA_EAR_1_1 ("resource/img/data/p_ear_1_1.png")//耳1-100（実績説明）
#define GH_DATA_EAR_1_2 ("resource/img/data/p_ear_1_2.png")//耳1-1000（実績説明）
#define GH_DATA_EAR_1_3 ("resource/img/data/p_ear_1_3.png")//耳1-10000（実績説明）
#define GH_DATA_EAR_1_4 ("resource/img/data/p_ear_1_4.png")//耳1-100000（実績説明）
#define GH_DATA_EAR_2_1 ("resource/img/data/p_ear_2_1.png")//耳2-100（実績説明）
#define GH_DATA_EAR_2_2 ("resource/img/data/p_ear_2_2.png")//耳2-1000（実績説明）
#define GH_DATA_EAR_2_3 ("resource/img/data/p_ear_2_3.png")//耳2-10000（実績説明）
#define GH_DATA_EAR_2_4 ("resource/img/data/p_ear_2_4.png")//耳2-100000（実績説明）
#define GH_DATA_EAR_3_1 ("resource/img/data/p_ear_3_1.png")//耳3-100（実績説明）
#define GH_DATA_EAR_3_2 ("resource/img/data/p_ear_3_2.png")//耳3-1000（実績説明）
#define GH_DATA_EAR_3_3 ("resource/img/data/p_ear_3_3.png")//耳3-10000（実績説明）
#define GH_DATA_EAR_3_4 ("resource/img/data/p_ear_3_4.png")//耳3-100000（実績説明）
#define GH_DATA_EAR_4_1 ("resource/img/data/p_ear_4_1.png")//耳4-100（実績説明）
#define GH_DATA_EAR_4_2 ("resource/img/data/p_ear_4_2.png")//耳4-1000（実績説明）
#define GH_DATA_EAR_4_3 ("resource/img/data/p_ear_4_3.png")//耳4-10000（実績説明）
#define GH_DATA_EAR_4_4 ("resource/img/data/p_ear_4_4.png")//耳4-100000（実績説明）
#define GH_DATA_EAR_5_1 ("resource/img/data/p_ear_5_1.png")//耳5-100（実績説明）
#define GH_DATA_EAR_5_2 ("resource/img/data/p_ear_5_2.png")//耳5-1000（実績説明）
#define GH_DATA_EAR_5_3 ("resource/img/data/p_ear_5_3.png")//耳5-10000（実績説明）
#define GH_DATA_EAR_5_4 ("resource/img/data/p_ear_5_4.png")//耳5-100000（実績説明）
#define GH_DATA_WEAPON_1 ("resource/img/data/p_weapon_1.png")//武器3（実績説明）
#define GH_DATA_WEAPON_2 ("resource/img/data/p_weapon_2.png")//武器4（実績説明）
#define GH_DATA_WEAPON_3 ("resource/img/data/p_weapon_3.png")//武器5（実績説明）
#define GH_DATA_WEAPON_4 ("resource/img/data/p_weapon_4.png")//武器6（実績説明）
#define GH_DATA_WEAPON_ALL ("resource/img/data/p_weapon_5.png")//全武器（実績説明）
#define GH_DATA_SCORE_1_1 ("resource/img/data/p_score_1_1.png")//スコア100000（実績説明）
#define GH_DATA_SCORE_1_2 ("resource/img/data/p_score_1_2.png")//スコア200000（実績説明）
#define GH_DATA_SCORE_1_3 ("resource/img/data/p_score_1_3.png")//スコア300000（実績説明）
#define GH_DATA_SCORE_1_4 ("resource/img/data/p_score_1_4.png")//スコア400000（実績説明）
#define GH_DATA_SCORE_1_5 ("resource/img/data/p_score_1_5.png")//スコア500000（実績説明）
#define GH_DATA_SCORE_1_6 ("resource/img/data/p_score_1_6.png")//スコア600000（実績説明）
#define GH_DATA_SCORE_1_7 ("resource/img/data/p_score_1_7.png")//スコア700000（実績説明）
#define GH_DATA_SCORE_1_8 ("resource/img/data/p_score_1_8.png")//スコア800000（実績説明）
#define GH_DATA_SCORE_1_9 ("resource/img/data/p_score_1_9.png")//スコア900000（実績説明）
#define GH_DATA_SCORE_1_10 ("resource/img/data/p_score_1_10.png")//スコア1000000（実績説明）
#define GH_DATA_SCORE_1_11 ("resource/img/data/p_score_1_11.png")//スコア5000000（実績説明）
#define GH_DATA_SCORE_1_12 ("resource/img/data/p_score_1_12.png")//スコア10000000（実績説明）
#define GH_DATA_SCORE_2_1 ("resource/img/data/p_score_2_1.png")//全スコア1000000（実績説明）
#define GH_DATA_SCORE_2_2 ("resource/img/data/p_score_2_2.png")//全スコア10000000（実績説明）
#define GH_DATA_SCORE_2_3 ("resource/img/data/p_score_2_3.png")//全スコア100000000（実績説明）
#define GH_DATA_SCORE_2_4 ("resource/img/data/p_score_2_4.png")//全スコア1000000000（実績説明）
#define GH_DATA_SCORE_2_5 ("resource/img/data/p_score_2_5.png")//全スコア10000000000（実績説明）
#define GH_DATA_SCORE_2_6 ("resource/img/data/p_score_2_6.png")//全スコア100000000000（実績説明）
#define GH_DATA_SCORE_2_7 ("resource/img/data/p_score_2_7.png")//全スコア1000000000000（実績説明）
#define GH_DATA_PERFORM_1 ("resource/img/data/p_perform_1.png")//実績10個開放（実績説明）
#define GH_DATA_PERFORM_2 ("resource/img/data/p_perform_2.png")//実績20個開放（実績説明）
#define GH_DATA_PERFORM_3 ("resource/img/data/p_perform_3.png")//実績30個開放（実績説明）
#define GH_DATA_PERFORM_4 ("resource/img/data/p_perform_4.png")//実績40個開放（実績説明）
#define GH_DATA_PERFORM_5 ("resource/img/data/p_perform_5.png")//実績50個開放（実績説明）
#define GH_DATA_PERFORM_6 ("resource/img/data/p_perform_6.png")//実績60個開放（実績説明）
#define GH_DATA_PERFORM_7 ("resource/img/data/p_perform_7.png")//実績70個開放（実績説明）
#define GH_DATA_PERFORM_ALL ("resource/img/data/p_perform_8.png")//実績全開放（実績説明）

#define GH_DATA_NON ("resource/img/data/p_non.png")//実績非開放（実績説明）

#define GH_ICON_RIGHTARROW ("resource/img/logo/arrow_r.png")//右矢印
#define GH_ICON_LEFTARROW ("resource/img/logo/arrow_l.png")//左矢印
#define GH_ICON_START ("resource/img/icon/S_start.png")//スタートボタン

//【音声ファイル】

#define SH_ATF ("resource/se/atf.ogg")//A.Tフィールド
#define SH_ATK ("resource/se/atk.ogg")//攻撃
#define SH_HIMEI ("resource/se/himei.ogg")//悲鳴
#define SH_OK ("resource/se/ok.ogg")//決定
#define SH_OMORI ("resource/se/omori.ogg")//大森アックスボンバー
#define SH_RENA ("resource/se/usodayo.ogg")//嘘だよッ！
#define SH_AexplSE ("resource/se/explosion3.mp3")//爆発
#define SH_AdestlSE ("resource/se/destruction1.mp3")//爆発仮
#define SH_AbroomingSE ("resource/se/brooming1.mp3")//爆発仮
#define SH_ABERASE ("resource/se/BERA.mp3")//べら仮
#define SH_SelectSE ("resource/se/Select1.mp3")//セレクト仮
#define SH_buttonSE ("resource/se/button.mp3")//セレクト仮

#define GUIDE_BOOK_SE1 ("resource/se/Book02.mp3")
#define GUIDE_BOOK_SE2 ("resource/se/Book02_1.mp3")
#define GUIDE_BOOK_SE3 ("resource/se/Book02_2.mp3")
#define click_SE ("resource/se/PC-Mouse.mp3")
#define roll_SE ("resource/se/roll.mp3")
#define Drum_roll_SE ("resource/se/Drum roll.mp3")
//【システムファイル】

#define SAVE_FILEDATA ("SaveData.dat")//セーブデータ
