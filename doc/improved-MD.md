# ɪMD: ɪmproved MarkDown

## Fundamental premises

Backward compatible with markdown, but not
necessarily with all extensions.

### ɪMD wish list

I wish from ɪMD:

 - ɪMD is not a "plain text format", but instead
   a written list of links:
     - locally,
     - to external resources,
 - it has all *important* facilities that is
   provided by HTML,
 - there is a simple editor with which you can
   edit this ɪMD, and in this editor you can
   add menues specific for the working directory,
   or a certain ɪMD-document,
 - you may configure a tool bar to start Python
   programs generating ɪMD

## Missing from MD

### Comments

### Wikimarkup-like tables

<pre style="background: white; margin-left: 2em">
{|
|- /*row1*/
||a1 /*col1*/
||a2 /*col2*/
|- /*row2*/
||b1 /*col1*/
||b2 /*col2*/
|}</pre>
or:
<pre style="background: white; margin-left: 2em">
-:↓   ↓
  a1  a2
  b1  b2
----</pre>
  gives
<table>
  <tr><td>a1</td><td>a2</td></tr>
  <tr><td>b1</td><td>b2</td></tr>
</table>

### File inclusions

`{:inc file.md}`

### Smarter lists

<ul>
      <li>continuations:
         <pre style="background: white; margin-left: 2em">
- blablabla
   - something here
   - something more here
  continuation after sublist here</pre>
      </li>
      <li>smart numbering:
<pre style="background: white; margin-left: 2em">
1. number
    a. something here
    b. something more here
   continuation after sublist here</pre>
      </li>
      with styles <code>type = 1 A a i I</code> and others
</ul>

### Variables

Variables: <code>{:set $a = 'text to insert'}</code> when something like:

<pre style="background: white; margin-left: 2em">
{:set $a = 'Mnemosyne'}

Then we arrived at the island $a, and we debarked.
  ···
On $a there are only two inns, one of a bad reputation,
and the other one worse.</pre>

gives:
<div style="background: #FFFFEE; margin-left: 2em">

Then we arrived at the island Mnemosyne, and we debarked.<br/>

···<br/>

On Mnemosyne there are only two inns, one of a bad reputation,
and the other one worse.

</div>

### Graphics:

- terminal windows, diagrams, boxes,

## Startup questions

- could it actually be grammarly defined?
    - perhaps those who claims it doesn't have ignored two
      things:
         - you may encode line-start and line-end
         - you may define grammars that handle flawed but
           not outrightly erroneous sentences in a
           systematic way
- drawing areas taking special drawing commands included in
  the IMD standard
