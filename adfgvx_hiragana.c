//ファイルに出力
//復号するときはキーワードを求めるようにする
//複合化するファイルが見つからなかったときの例外処理
//fopenの引数部分「, css = UTF-8」部分大事

/*

一次大戦中にドイツ軍で使用されていた『ADFGVX暗号』を参考にした暗号化プログラムです。ADFGVX暗号では平文を「A」「D」「F」「G」「V」「X」の６文字で暗号化しますが、
このプログラムでは平文を「き」「さ」「ま」「み」「て」「い」「る」「な」「！」の９文字で暗号化します。
使用可能な文字は平仮名と[!, ?, 「, 」, (, )]←の記号です。

*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h> 
#include <wchar.h>

int itsEndIndexNum(wchar_t string[]); //指定したワイド文字列配列の中で一番後ろにある文字のインデックス番号を返す.一文字も入ってない場合-1を返す.
int NumberOnTable(wchar_t word); //指定した平仮名がひらがな表の[0][0]の要素を「1」、[8][8]の要素を「81」として、その規則に従った値を返す。見つからなかったら-1を返す。
void copyArray(wchar_t assigned[], wchar_t exist[]); //要素数20のint型リストの全要素をを9で初期化
void setArray(int array[]); //整数型の配列を初期化するための関数
void sortHiragana(wchar_t sortedArray[]); //引数に指定した配列をひらがな順にソートする。

wchar_t table[9][9] ={
            {L'あ',L'い',L'う',L'え',L'お',L'か',L'き',L'く',L'け'},
            {L'こ',L'さ',L'し',L'す',L'せ',L'そ',L'た',L'ち',L'つ'},
            {L'て',L'と',L'な',L'に',L'ぬ',L'ね',L'の',L'は',L'ひ'},
            {L'ふ',L'へ',L'ほ',L'ま',L'み',L'む',L'め',L'も',L'や'},
            {L'ゆ',L'よ',L'ら',L'り',L'る',L'れ',L'ろ',L'わ',L'を'},
            {L'ん',L'が',L'ぎ',L'ぐ',L'げ',L'ご',L'ざ',L'じ',L'ず'},
            {L'ぜ',L'ぞ',L'だ',L'ぢ',L'づ',L'で',L'ど',L'ば',L'び'},
            {L'ぶ',L'べ',L'ぼ',L'ぱ',L'ぴ',L'ぷ',L'ぺ',L'ぽ',L'っ'},
            {L'ゃ',L'ゅ',L'ょ',L'！',L'？',L'「',L'」',L'（',L'）'}
        };

int main(int argc, const char * argv[]){
    setlocale(LC_ALL, "ja_JP.UTF-8"); //文字コードを設定
    //--------換字をするための準備----------
    wchar_t transformKey[10] = {L'き',L'さ',L'ま',L'み',L'て',L'い',L'る',L'な',L'!','\0'};
    wchar_t pwdToArrange[11] = {L'\0'}; //パスワードは10文字以内
    wchar_t text[501] = {L'\0'}; //暗号化する文章を入れるやつ
    wchar_t codedText[50][20] = { {L'\0'} }; //暗号化した文章を入れるやつ
    wchar_t decodedText[100][10] = { {L'\0'} }; //復号化した文章を入れるやつ
    wchar_t sorted_pwdToArrange[11] = {L'\0'}; //パスワードをあいうえお順にしたものを入れるやつ
    char fName[21] = {'\0'};
    int findFlag = 0;
    int count = 0; //設定したパスワードの1文字目から、あいうえお順にしたときに何番目にあるかを調べるときに使う
    int rowIncrement = 0;
    //------------------------------------
    int mode = 9;
    printf("-----------------モードを選んでください-----------------\n");
    printf("0 : 暗号化\n");
    printf("1 : 復号化\n");
    printf("---------------------------------------------------------\n");
    printf("モード? : ");
    scanf("%d", &mode);

    switch (mode){
        case 0:
            printf("-----------------パスワードを設定してください-----------------\n");
            printf("＊10文字以内\n");
            printf("＊ひらがなのみ\n");
            printf("---------------------------------------------------------\n");
            printf("パスワード? : ");
            scanf("%ls",pwdToArrange);

            //------入力されたパスワードをあいうえお順にソートする------
            copyArray(sorted_pwdToArrange, pwdToArrange); //sorted_pwdToArrangeへpwdToArrangeの要素をコピー
            sortHiragana(sorted_pwdToArrange);
            //--------------------------------------------------

            printf("-----------------文を入力してください。(qで終了)-----------------\n");
            printf("使用可能文字: ひらがな\n");
            printf("使用可能文字: ！, ？, 」,「, ）,（　 ←すべて全角\n");
            printf("注:全角空白を入れるとバグります。直したい。\n");
            printf("----------------------↓入力してください(qで終了)------------------------\n");

            int indexToInput = 0; ////scanfを無限ループして,改行処理をするために使う
            while(1){ ////scanfを無限ループ
                indexToInput = itsEndIndexNum(text); ////scanfする前の最後尾にある文字のインデックス番号を代入
                scanf("%ls", &text[indexToInput + 1]);
                indexToInput = itsEndIndexNum(text); ////scanf後の最後尾にある文字のインデックス番号を代入
                if(text[indexToInput] == 'q'){ //qが入力されたらループ終了
                    break;
                }
            }

            for(int i = 0; i < sizeof(text); i++){ //textの文字をひとつずつチェック

                if(text[i] == 'q'){ //textから取り出した値が'\0'だったら全ての文字を見終えたのでtext内の探索を中断する
                    printf("\n");
                    break;
                }

                for(int r = 0; r < 9; r++){  //textから取り出した文字が表の何行何列にあるかを確かめる r=行 c=列
                    for(int c = 0; c < 9; c++){
                        if(text[i] == table[r][c]){ //textから取り出した文字が見つかったら
                                for(int j = 0; sorted_pwdToArrange[j] != '\0'; j++){ //arrangeとsortが一致するまで
                                    if(pwdToArrange[count] == sorted_pwdToArrange[j]){ //設定したパスワードの、指定した位置にある文字と、それをあいうえお順にしたときの位置が等しいiになったら
                                        if(codedText[rowIncrement][2*j] == '\0'){ //パスワードに同じ平仮名があったら上書きしてしまうので,重複の場合は2個後ろの要素に入れる
                                            codedText[rowIncrement][2*j] = transformKey[r];
                                            codedText[rowIncrement][2*j+1] = transformKey[c];
                                        } else if(codedText[rowIncrement][2*j+2] == '\0'){
                                            codedText[rowIncrement][2*j+2] = transformKey[r];
                                            codedText[rowIncrement][2*j+3] = transformKey[c];
                                        } else{ //重複が3文字あったとき用
                                            codedText[rowIncrement][2*j+4] = transformKey[r];
                                            codedText[rowIncrement][2*j+5] = transformKey[c];
                                        }
                                        break;
                                    }
                                }

                                findFlag = 1; //表から文字が見つかったらそれを出力してflagを立てる
                                count++;//パスワードの次の文字へ
                                if(pwdToArrange[count] == '\0'){ //パスワードの末尾になったら、次の行へ
                                    count = 0;
                                    rowIncrement++;
                                }
                                break; //列のインクリメントを中断
                        }
                    }
                    if(findFlag == 1){ //値が見つかったら,行のインクリメントを中断
                        findFlag = 0;
                        break;
                    }
                }
            }

            rowIncrement++;
            codedText[rowIncrement][0] = 'q';

            for(int r = 0; codedText[r][0] != 'q'; r++){
                for(int c = 0; pwdToArrange[c] != '\0'; c++){
                    wprintf(L"%lc%lc", codedText[r][2*c], codedText[r][2*c+1]);
                }
            }

            printf("\n");

            char ans = '\0';
            printf("暗号文をファイルに保存しますか？(y or n) : ");
            scanf("%s", &ans);
            if(ans == 'y'){
                FILE *fp;
                int codedKey[20] = { 0 }; //暗号化したパスワードを保存するための配列。パスワードはひらがなを行列番号で表現することで暗号化する.　あ→00 
                setArray(codedKey); //ひらがなの表は[8][8]までしかなく、9は使われないので9で初期化しておく
                int flag_2 = 0;
                for(int i = 0; pwdToArrange[i] != '\0'; i++){ //パスワードのひらがなを順に行列番号で表現する.　あ→00
                    for(int r = 0; r < 9; r++){  //textから取り出した文字が表の何行何列にあるかを確かめる r=行 c=列
                        for(int c = 0; c < 9; c++){
                            if(pwdToArrange[i] == table[r][c]){ //textから取り出した文字が見つかったら
                                codedKey[2*i] = r; //行番号
                                codedKey[2*i+1] = c; //列番号
                                flag_2 = 1;
                                break;
                            }
                        }
                        if(flag_2 == 1){
                            flag_2 = 0;
                            break;
                        }
                    }
                }
                
                printf("保存するファイルの名前を入力してください(20文字以内, 半角英数字のみ, 末尾に「.txt」をつけてください) : ");
                scanf("%s", fName);
                fp = fopen(fName, "w, ccs = UTF-8");
                for(int i = 0; codedKey[i] != 9; i++){
                    fprintf(fp, "%d", codedKey[i]);
                }
                fprintf(fp, "\n");
                for(int r = 0; codedText[r][0] != 'q'; r++){ // ファイルに暗号文を保存する
                    for(int c = 0; pwdToArrange[c] != '\0'; c++){
                        fwprintf(fp, L"%lc%lc", codedText[r][2*c], codedText[r][2*c+1]);
                    }
                }
                fprintf(fp, "\n");
                fclose(fp);
            }
            
            printf("\n");    
            break;
        
        case 1:
            printf("-----------------復号化するファイル名を入力してください-----------------\n");
            printf("ファイル名? : ");
            scanf("%s",fName);

            FILE *fp;
            fp = fopen(fName, "r, ccs = UTF-8");
            char codedKey_char[21] = {'\0'}; //暗号化したパスワードを保存するための配列。パスワードはひらがなを行列番号で表現することで暗号化する.　あ→00 
            int row = 0;
            int column = 0;
            fgets(codedKey_char, 21, fp); //codedKeyへ暗号化されたパスワードを代入
            for(int i = 0; codedKey_char[2*i] != '\n'; i++){ // \nが出てくるまで暗号化されたパスワードを参照する
                row = codedKey_char[2*i] - '0';
                column = codedKey_char[2*i + 1] - '0';
                pwdToArrange[i] = table[row][column]; //pwdToArrangeへ設定したパスワードを復号化して代入
            }
            
            wchar_t checkPwd[11] = {L'\0'}; //ユーザが入力したpwdを入れる。それがpwdToArrangeと一致するか確認
            printf("-----------------パスワードを入力してください-----------------\n");
            printf("＊10文字以内\n");
            printf("＊ひらがなのみ\n");
            printf("---------------------------------------------------------\n");
            int flag_3 = 0;
        // -------------checkPwdとpwdToArrangeが一致するかを確認する部分---------------------- 
            while(1){
                printf("パスワード? : ");
                scanf("%ls",checkPwd);
                if(checkPwd[0] == 'q'){
                    return 0;
                }
                for(int i = 0; i < 11; i++){ // checkPwdとpwdToArrangeが一致するかを確認
                    if(checkPwd[i] != pwdToArrange[i]){
                        printf("パスワードが違います。再入力してください。辞める場合はqを入力。\n");
                        for(int i = 0; checkPwd[i] != '\0'; i++){ // checkPwdを'\0'で初期化
                            checkPwd[i] = '\0';
                        }
                        break;
                    }
                    if(i == 10){ //全部一致したらflag3を立てる
                        flag_3 = 1;
                        break;
                    }
                }
                if(flag_3 == 1){ //flag3が立ってたらwhileを脱出
                    break;
                }
            }
        // --------------------------------------------------------------------------------- 
        // ----------------------暗号文を読み込み,配列(getCode)に入れる部分--------------------------------
            wchar_t tempArray[1000] = {'\0'};
            fgetws(tempArray, 1000, fp); //codedKeyへ暗号化されたパスワードを代入. なぜか値がとびとびで入るんだけど？
            fclose(fp);
            wchar_t getCode[1000] = {'\0'};
            int index = 0;
            for(int i = 0; tempArray[i] != '\n' ; i++){ //値がとびとびで代入されるため、tempArrayの要素を'\n'が出てくるまで'\0'を飛ばして、getCodeへ代入。
                if(tempArray[i] == '\0') continue;                                                //↑ 暗号を保存する際,最後に改行するようにしてあるので、それが暗号文の末尾の目印となる.
                getCode[index] = tempArray[i];
                index++;
            }
        // --------------------------------------------------------------------------------- 

        copyArray(sorted_pwdToArrange, pwdToArrange); //sorted_pwdToArrangeへpwdToArrangeをコピー
        sortHiragana(sorted_pwdToArrange); //sorted_pwdToArrangeをあいうえお順にソート
        //--------------------復号化-------------------------------
        int r = 9; //暗号文を平文に戻す際、暗号文がひらがな表の何行目かを入れる　ex)さ　→ 1  9は使われないので9で初期化
        int c = 9; //暗号文を平文に戻す際、暗号文がひらがな表の何列目かを入れる　ex)き　→ 0
            for(int i = 0; getCode[2*i] != '\0'; i++){ //暗号文を２文字ずつ見ていく
                //----------暗号文2文字をひらがな1文字へ復号する部分--------------------------
                for(int j = 0; transformKey[j] != '\0'; j++){
                    if(transformKey[j] == getCode[2*i]){ // getCode[2*i]がtansformKeyの何番目（この値がひらがな表の行または列を表す）にあるのかを判定
                        r = j;
                    }
                    if(transformKey[j] == getCode[2*i + 1]){
                        c = j;
                    }
                    if(r != 9 && c != 9){
                        break;
                    }
                }//---------------------------------------------------------

                //--------------復号化したひらがなをパスワードに従った順番に並べ替える部分-----------
                for(int k = 0; pwdToArrange[k] != '\0'; k++){
                    if(sorted_pwdToArrange[count] == pwdToArrange[k]){
                        decodedText[rowIncrement][k] = table[r][c];
                        count++;
                        r = 9; //初期化し直す
                        c = 9; //初期化し直す
                        break;
                    }
                }

                if(sorted_pwdToArrange[count] == '\0'){
                    count = 0;
                    rowIncrement++;
                }
                
            } //--------------------復号化おわり----------------------------
            decodedText[rowIncrement + 1][0] = 'q'; //復号文の終わりの目印用にqを代入しておく

            for(int r = 0; decodedText[r][0] != 'q'; r++){
                for(int c = 0; c < 10; c++){
                    if(decodedText[r][c] != '\0'){
                        printf("%lc",decodedText[r][c]);
                    }
                }
            }
            printf("\n");
            break;
    }
    return 0;
}


int itsEndIndexNum(wchar_t string[]){
    int count;
    for(count = 0; string[count] != '\0'; count++){

    }
    return count - 1; //配列の最後尾にある文字のインデックス番号を返す.
}

int NumberOnTable(wchar_t word){
    int count = 0;

     for(int r = 0; r < 9; r++){  //ひらがな表探索
            for(int c = 0; c < 9; c++){ //ひらがな表探索
                count++;
                if(word == table[r][c]){
                    return count;
                }
            }
        }
    
    return -1;
}

void copyArray(wchar_t assigned[], wchar_t exist[]){
    for(int i = 0; exist[i] != '\0'; i++){
        assigned[i] = exist[i];
    }
}

void setArray(int array[]){
    for(int i = 0; i < 20; i++){
        array[i] = 9;
    }
}

void sortHiragana(wchar_t sortedArray[]){
    wchar_t tmp = '\0';
    int preNum = 0; //ひらがな表の位置を入れる
    int postNum = 0; //ひらがな表の位置を入れる
    for(int i=0; i < itsEndIndexNum(sortedArray) + 2; i++){ //バブルソート
        for(int j=0; j < itsEndIndexNum(sortedArray) ; j++){ //　隣同士の比較を行う
            preNum = NumberOnTable(sortedArray[j]);
            postNum = NumberOnTable(sortedArray[j+1]); 
            if(preNum > postNum){
                tmp = sortedArray[j];
                sortedArray[j] = sortedArray[j+1];
                sortedArray[j+1] = tmp;
            }
        }
    }
}

