機人~Kijin~
【制作者】　グループ(自身含め５人)
【制作日】　３年後期
【種　別】　ゲーム
【動作環境】Windows10
【開発環境】
　　VisualStudio2022
　　DxLib
　　Unity
　　Effekseer
―――――――――――――――――――――――――――――――――――――

・実行に必要なプログラムとデータファイル
　　"Project"ファイル内の.exeから実行してください。
・開発に使用したソースファイル
　　"Project"ファイル内にあります。
・実行画面をキャプチャした動画ファイル
　　"Video"ファイル内にあります。


・操作方法
	WASD			自機移動
	マウスカーソル		カメラ視点操作
	左クリック		攻撃
	LSHIFT	      ダッシュ
	Eキー			必殺技
	SPACE長押し		ホバリング


・制作ヵ所
　　・全般担当
	Keyboardクラス
	Padクラス
	Controllerクラス
	PlayerAnimIndex.h
	PlayerBehaviorクラス（デザインパターンは委託）
	PlayerAttackBehaviorクラス（デザインパターンは委託）
	PlayerSkillBehaviorクラス（デザインパターンは委託）
　　・一部担当
	Mathクラス（Clamp()とDirNearAroundRad()を担当）
	Animatorクラス（デザインパターンは委託、アニメーションブレンドはチームメイトと共同で制作）
	シェーダー全般（ほかのチームメイトが担当していたが、勉強のために制作に一部立ち会った）
	UI全般（制作後にほかのチームメイトがデザインパターンを変更した）