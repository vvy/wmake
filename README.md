<p><span style="font-size: 14pt;">A simulation of make in Linux for learning.Instead of "makefile", it uses "wmakefile".</span></p>
<p><span style="font-size: 14pt;">A blog (in Chinese) about how to develop it:<a href="http://www.cnblogs.com/wuyuegb2312/p/3433931.html">http://www.cnblogs.com/wuyuegb2312/p/3433931.html</a></span></p>
<ul>
<li><span style="font-size: 18px;">Deal with at most <span style="color: #339966;">MAXCOMMAND</span> lines of command after each target line.</span></li>
<li><span style="font-size: 18px;">File dependency is transformed&nbsp;to a&nbsp;directed graph, which can run a dfs algorithm to detective a cycle in it.</span></li>
<li><span style="font-size: 18px;">Use hash table for graph.</span></li>
<li><span style="font-size: 18px;">Support 'wmake clean'.</span></li>
<li><span style="font-size: 18px;">Don't support&nbsp;variables.</span></li>
<li><span style="font-size: 18px;">folder 'wmake_sample' for test.</span></li>
</ul>
<p>&nbsp;</p>
<p><span style="font-size: 18px;">Update:</span></p>
<p><span style="font-size: 18px;">2013</span></p>
<p><span style="font-size: 18px;">11.22</span></p>
<p><span style="font-size: 18px;">　　-Increased the number of command lines supprot from only one to a&nbsp;macro definition MAXCOMMAND.</span></p>
<p><span style="font-size: 18px;">　　-Support new format in wmakefile, there can be any number of space between the ':' in a dependency line.</span></p>
<p><span style="font-size: 18px;">　　-Instead of a .zip file, use case now is a whole folder.</span></p>
<p>&nbsp;</p>
<p><span style="font-size: 18px;">11.18</span></p>
<p><span style="font-size: 18px;">　　-First version.</span></p>
