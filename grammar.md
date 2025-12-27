$$
\begin{align*}
\texttt{program} &\rightarrow \texttt{function\text{*}} \\
\texttt{function} &\rightarrow \texttt{type identifier "(" parameters ")" "{" statement\text{*} "}"} \\
\texttt{parameters} &\rightarrow (\texttt{type identifier} ("," \texttt{type identifier})\text{*})? \\
\texttt{statement} &\rightarrow \texttt{var\_decl} \mid \texttt{assignment} \mid \texttt{if\_stmt} \mid \texttt{while\_stmt} \mid \texttt{return\_stmt} \mid \texttt{expr\_stmt} \\
\texttt{var\_decl} &\rightarrow \texttt{type identifier} ("=" \texttt{expression})? ";" \\
\texttt{assignment} &\rightarrow \texttt{identifier "=" expression ";"} \\
\texttt{if\_stmt} &\rightarrow \texttt{"if" "(" expression ")" "{" statement\text{*} "}"} (\texttt{"else" "{" statement\text{*} "}"} )? \\
\texttt{while\_stmt} &\rightarrow \texttt{"while" "(" expression ")" "{" statement\text{*} "}"} \\
\texttt{return\_stmt} &\rightarrow \texttt{"return" expression ";"} \\
\texttt{expression} &\rightarrow \texttt{equality} \\
\texttt{equality} &\rightarrow \texttt{comparison} (("==" \mid "!=") \texttt{comparison})\text{*} \\
\texttt{comparison} &\rightarrow \texttt{term} (("<" \mid ">" \mid "<=" \mid ">=") \texttt{term})\text{*} \\
\texttt{term} &\rightarrow \texttt{factor} (("+" \mid "-") \texttt{factor})\text{*} \\
\texttt{factor} &\rightarrow \texttt{unary} (("*" \mid "/") \texttt{unary})\text{*} \\
\texttt{unary} &\rightarrow ("-" \mid "!") \texttt{unary} \mid \texttt{primary} \\
\texttt{primary} &\rightarrow \texttt{NUMBER} \mid \texttt{IDENT} \mid "(" \texttt{expression} ")" \mid \texttt{CHAR} \mid \texttt{STRING}
\end{align*}
$$
