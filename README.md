# libtempura

Tempura is an HTML templating library (or at least, it will be when it's finished).

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)

Web developers already have a wealth of template engines to choose from, don't they? Or do they...? Many template engines are written in web scripting languages and as such are suitable for use only by developers who also use that language. Tempura is written in C and compiled to machine code, because it aims to be usable in any context. Libtempura bindings could be written for PHP, Python, Ruby, NodeJS or any future language. Tempura could even be used at the command line and integrated into practically any workflow.

### Language design

Some template engines treat the input as a generic string and process dynamic content only where certain delimiting tokens are found. Drawbacks of this approach are that the developer still has to write HTML with all of its verbosity, and that the whitespace and comments included for readability still end up being sent to the browser. Tempura's templating language is designed to be succinct (no more closing tags), and its output will be minified by default because whitespace and comments are ignored at the lexing stage. Tempura can even figure out whether quotes around your attribute values are required or not; because even with gzipped content, there are always more bytes to save.

Tempura is also designed with security in mind. Because the syntax is context aware, Tempura knows whether a string or variable needs to be escaped as an HTML attribute or as a text node, so you can forget about XSS vulnerablities. Future scope for the language could also include automatic CSS and JavaScript escaping.

Here's an example of some basic Tempura code:
```
html {
	head {
    	meta @charset: 'UTF-8';
        title 'Hello, World!';
    }
    body {
    	h1 'Hello, World!';
        hr;
        p 'Lorem ipsum dolor sit amet.';
    }
}
```

An opening HTML element uses braces to enclose its child nodes. The braces can be omitted if there's only one child, just like `if`/`else`, `for` and `while` blocks in C and other languages. An element with no children is empty, so `hr;` in Tempura (or `hr{}`, which is equivalent) will be rendered as `<hr />` in XHTML mode or `<hr>` (without a closing tag) in HTML mode.

HTML child nodes are like statements in an imperative programming language; strings  are quoted and they are terminated with a semicolon. HTML attributes are denoted with an `@`-sign and a colon separates the name from its value.

Tempura is fully forward-compatible with new HTML tags and attributes because it treats any valid identifier as such. This also means it has no trouble with `data-` attributes. Tempura also doesn't know which elements are supposed to be self-closing; this is achieved naturally because empty elements are recognized as such. The Tempura code: `img { @src: '/logo.png'; @alt: 'Logo'; }` defines only HTML attributes as children of the `img` node, so no closing `</img>` tag is produced. To create an empty element that doesn't behave this way, you can simply add an empty string as a child node: the Tempura code `div '';` will be interpreted as `<div></div>`.

Tempura also features shorthand syntax for specifying element `class`es and `id`s. The `.` and `#` sigils are borrowed from CSS and used after element names:
```
div#outer {
	div.inner 'foo';
    div.inner 'bar';
}
```

Multiple classes can be specified by chaining them together. The code: `div#foo.bar.baz 'qux';` will be interpreted as `<div id=foo class="bar baz">qux</div>`;

### Variables, functions and imports

A template engine wouldn't be of much use without dynamic content, so Tempura supports string, array and object variables, denoted by the sigil `$` as in Bash, Perl, PHP and other languages.

Functions are employed for reusable markup and Tempura also supports a 'foreach' construct to iterate through array items:
```
function block($heading, $content) {
	div.block-container {
    	div.block-heading $heading;
        div.block-content $content;
    }
}

html {
	head { meta @charset: 'UTF-8'; title $title; }
    body div#blocks {
        foreach ($blocks as $block) {
            block($block.heading, $block.content);
        }
    }
}
```

Object property access is achieved using the `.` operator, and functions may also specify default parameter values, which will be used if the function call specifies `default` instead of a variable or literal value:
```
function foo($bar, $baz = 'BAZ', $qux) {
	span {
    	$bar; $baz; $qux;
    }
}
foo('BAR', default, 'QUX');
```

Note that unlike many programming languages, Tempura does not impose any limitations on which or how many parameters are specified with default values, since use of those default values is explicit.
