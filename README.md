<h1>C++ Hyperspace Library</h1>

The library consist of several classes that enable writing readable and efficient C++ code for programs that need high-dimensional grids of cells.

The code is written completely in standard C++ and requires a C++-11 compiler.

<h2>Rationale</h2>

The main benefit of the library is the <i>readability</i> of the programming code that handles data in a multi-dimensional space consisting of a number of neighboring cells. The library provides a simple and consistent programming interface for accessing all cells and their respective neighbors. Moreover, it does this in high-dimensional spaces of arbitrary size (limited only with available computer memory) and in constant time. Currently the Moore's neighborhoods of range 1 are supported.


<h2>Complexity</h2>

In the fixed D-dimensional space, the complexity of operations is as follows:
<ul>
    <li>Random access:<br> 
        - using linear index: constant, O(1)<br>
        - using coordinates: constant, O(D) (linear on the constant number of dimensions D)<p>
    </li>
    <li>Random access to the cell's neighboring cells:<br>
        - with knowledge of the cell's type: constant, O(1)<br>
        - without knowledge of the cell's type: constant, O(D) (linear on the constant number of dimensions D)
    </li>
</ul>

<h2>Installation</h2>
The library consists of a single header file (<i>hyper.h</i>), that can be installed/included either:
<ol>
    <li>locally in the project (<i>#include "./relative/path/to/hyper.h"</i>), or</li>
    <li>system-wide (<i>#include &lt;sprogar/hyper.h&gt;</i>)</li>
</ol>

The former usage is simpler and requires no administrator privileges, the latter approach, however, normally requires administrator's rights to create and put the header in the appropriate folder (e.g. '/usr/local/include/sprogar').



<h2>Usage</h2>
All the classes are within the <i>sprogar::hyper</i> namespace:
<p>
    <i>
        #include &lt;sprogar/hyper.h&gt; // if installed globally, or<br>
        #include "hyper.h" // if installed locally<br>
        <br>
        using namespace sprogar::hyper;<br>
    </i>
</p>

The library provides two main ways for using and handling multi-dimensional spaces:
<ol>
    <li>Convenient containers of arbitrary type T:
        <p><i>
            wrapped_space&lt;int, 5, 7&gt; spc{0};<br>
            spc(3, 4) = 42;<br>
        </i></p>
    </li>
    <li>STL-like iterators for use with any linear random-access container (e.g. <i>std::vector&lt;&gt;</i> or <i>std::array&lt;&gt;</i>):
        <p>
            <i>
                std::array&lt;int, 5 * 7&gt; spc = {0};<br>
                location_iterator&lt;5, 7&gt; it(3, 4);<br>
                spc[it] = 42;  // or spc[25] = 42;
            </i>
        </p>
    </li>
</ol>

Current version of the Hyperspace library is only suited for projects where the size of the programmed spaces is known at compile time, what is true for most projects requiring an optimized C++ library anyway. The benefits of having the dimensions predefined include more exhaustive compiler optimizations, type checks and better performance of the resulting code.


<h3>Hyper-containers</h3>
The provided containers are constructed similar to the STL's <i>std::array&lt;T, N&gt;</i> container, except they allow for more than 1 dimension to be used simply by providing <i>any</i> number of fixed-size dimensions: <i>wrapped_space&lt;T, N<sub>D</sub>, N<sub>D-1</sub>, ..., N<sub>1</sub>, N<sub>0</sub>&gt</i>.
<p>
Currently, the library provides two types of containers according to how the neighbors are determined:
<ol>
    <li><i>unwrapped-spaces</i>, where a cell on the dimension's borders has no neighbors in the corresponding direction; and</li>
    <li><i>wrapped-spaces</i>, where the cells at positions <i>0</i> and <i>N<sub>i</sub>&nbsp;-&nbsp;1</i> are considered to be neighbors.</li>
</ol>

For example, in the <i>unwrapped_space&lt;T, 10, 10&gt;</i> the first cell (0,&nbsp;0) has exactly 3 neighbors {(0,&nbsp;1), (1,&nbsp;0) and (1,&nbsp;1)}; in the <i>wrapped_space&lt;T, 10, 10&gt;</i>, however, its Moore neighborhood consists of 8 cells: {(0,&nbsp;1), (0,&nbsp;9), (1,&nbsp;0), (1,&nbsp;1), (1,&nbsp;9), (9,&nbsp;0), (9,&nbsp;1) and (9,&nbsp;9)}.


<h3>Iterators</h3>
Iterators are the standard way to traverse any container. The iterators provided in this library can be used both on hyper-containers as well as with any other linear-addressing-type containers (for example the standard C array [], or STL's std::vector&lt;&gt;...). They map the corresponding multi-dimensional coordinate into a universal 1D coordinate. For examples please inspect the provided test scenarios.
<p>
The iterators can be obtained either by (1) normal construction or (2) via the <i>begin()</i> method; this allows also range-for loops to be used both for traversing the space and particular cell's neighboring cells. The iterators through the space allow, as always, to retrieve the content of the cell via the <i>*&nbsp;operator</i> and forward movement by the prefix <i>++&nbsp;operator</i>. Additionally, they provide access to the list of neighboring cells either through offsets relative to the iterator's position, or a reference to the neighboring cell.
<p>
Please see the accompanying tests and examples for how exactly to use them.


<h2>Tests</h2>
The project includes plenty of self-evident tests, which document all basic capabilities of provided classes. All tests are written as simplest functions in an initializer list to a <i>std::vector&ltvoid (*pf)()&gt;</i>, allowing for simple execution, easy expansion and maintenance/refactoring.
<p>
The <i>test.cpp</i> file is included in a separate subfolder with all the tests packaged within the <i>sprogar::test</i> namespace.



<h2>Examples</h2>
The first example is the standard 2D Game Of Life, the most popular cellular automaton display. A typical program (in <a href="http://rosettacode.org/wiki/Conway%27s_Game_of_Life">any</a> language) is not simple to comprehend and the Rosetta C++ solution is just awful. The code based on the Hyperspace library presented here, however, is short, intuitive and easy to read.
<p>
Next, porting any 2D implementation to higher dimensions (3D, 4D...) is traditionally difficult and prone to errors. The hyperspace-enabled game-of-life example, on the contrary, is <i>extremely</i> easy to extend to 3D simply by
<i>adding the 3rd dimension</i> to the definition of the grid object!
<p>
This is shown in the second example (<i>game_of_life_3D()</i>), where most of the program code from the 2D example remains intact. In fact, the core CA handling code remains completely unaltered! Any higher dimension can be achieved the same way.


<h2>Test and demonstration</h2>
In order to run the accompanying tests and examples, a simplistic main file is provided. Please follow the compiling instructions there-in.

<h2>To-Do List</h2>
<ol>
    <li>Other types of neighborhoods (beside <a href="https://en.wikipedia.org/wiki/Moore_neighborhood">Moore's</a>)
    <li>API improvements</li>
    <li>Documentation</li>
</ol>


<h2>Known issues</h2>
<ol>
<li>The compiler fails when <i>auto</i> keyword is used in place of the exact typename when using the range-for syntax in neighborhood traversal. The workaround is currently unknown, suggestions/corrections welcome! (see ISSUE #1 in test.cpp)</li>
<li>Because of the properties of the STL's specialization of the <i>std::vector&lt;bool&gt;</i> class, use of booleans as space contents (<i>space&lt;bool&gt</i>) prevents traversal of neighbors using the range-for syntax. Current workaround is to use <i>int</i> in place of <i>bool</i> (see ISSUE #2 in test.cpp).</li>
</ol>