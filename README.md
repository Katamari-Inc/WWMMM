World Wide Maze Moving Model
=====


[dotFes 2013 京都](http://www.dotfes.jp/2013kyoto/) に出展するインスタレーションの一部、WWMMM の oF 側のコードようリポジトリ。その他のファイル、設計図とか 3D プリントしたパーツのデータとかは[こっち WWMMM-assets](https://github.com/Katamari-Inc/WWMMM-assets)。

- mapamok08-wwmmm: 本番用。
- mapamok08: oF v0.8 対応した [mapamok](https://github.com/YCAMInterlab/ProCamToolkit/wiki/mapamok-(%E6%97%A5%E6%9C%AC%E8%AA%9E)。これベースでモデルうごかす。
- mapamok_with_Arduino_walkthrough: [これ](https://github.com/YCAMInterlab/ProCamToolkit/wiki/mapamok-with-Arduino-walkthrough)やってみた。

_ からはじまるやつはちょっとしたテスト。

- _SerialCommand: ofSerial で Arduino コントロールしてみるテスト。
- _EnvMap: [ofxCubeMap](https://github.com/andreasmuller/ofxCubeMap) つかって環境マップしてみるテスト。
- _Leaped2: Leap Motion でスチレンボードをコントロールするテスト。
- _SyncedStage: OSC で quaternion うけとって回転させるテスト。
- _MovingProjection: mapamok でモーターと連動して動くオブジェクトにプロジェクションしてみるテスト。オブジェクトの回転は OSC でうけとれる。
- _SendOrientation: _MovingProjection にダミーの回転データをおくりつけてみるテスト。
- _StageRenderer: ステージを oF でちゃんとレンダリングするテスト。
- _ConstancAcc: L6470 の動作シミュレーションてすと。等加速度運動。 

[メイキングブログはこつら。](http://mowwmmm.tumblr.com/)
