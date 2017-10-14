/**
 * Created by chery on 14/10/2017.
 */
const fs = require('fs');
const readLine = require('readline');

const rl = readLine.createInterface({
    input: fs.createReadStream('Lab6.2.13.cpp')
});

const ifEnum = {
    IFCOND: 0,
    CASECOND: 1,
    TERNCOND: 2,
    ELSECOND: 3
};

const counter = {
    op: 0,
    condOp: 0,
    maxNesting: 0,
    nestingStack: [],
    updateMaxNesting: function () {
        let len = this.nestingStack.length - 1;
        this.maxNesting = this.maxNesting < len ? len : this.maxNesting;
    }
};

rl.on('line', function (line) {

    //Remove all strings
    for (let startPos = line.indexOf('\"'), pos = 0; line.includes('\"', pos); ) {
        pos = line.indexOf('\"', pos) + 1;
        if (!line.includes('\"', pos)) {
            line = line.slice(0, startPos) + line.slice(pos);
        }
    }

    //Remove all comments
    if (line.includes('//')) {
        line = line.slice(0, line.indexOf('//'));
    }

    //Increase nesting if 'else'
    if (line.includes('else') && line.includes('{')) {
        counter.nestingStack.push(ifEnum.ELSECOND);
    }

    if (line.includes('if') || line.includes('for') || line.includes('while') || line.includes('case') || line.includes('?')) {
       counter.condOp++;
       counter.op++;

       //Update nesting stack
       if (line.includes('if') || line.includes('for') || line.includes('while')) {
           counter.nestingStack.push(ifEnum.IFCOND);
           if (!line.includes('{')) {
               counter.updateMaxNesting();
               counter.nestingStack.pop();
           }
       }
       else if (line.includes('case')) counter.nestingStack.push(ifEnum.CASECOND);
       else counter.nestingStack.push(ifEnum.TERNCOND);
       counter.updateMaxNesting();
    }

    //Update nesting stack due to end of ternary
    if (line.includes(':') && counter.nestingStack.length > 0 && counter.nestingStack[counter.nestingStack.length - 1] === ifEnum.TERNCOND)
        counter.nestingStack.pop();

    //Update nesting stack due to end of clojure
    if (line.includes('}') && counter.nestingStack.length > 0) {
        let len = counter.nestingStack.length;

        if (counter.nestingStack[len - 1] === ifEnum.ELSECOND)
            for (;counter.nestingStack[len - 1] === ifEnum.ELSECOND; len--)
                counter.nestingStack.pop();
        else if (counter.nestingStack[len - 1] === ifEnum.CASECOND)
            for (;counter.nestingStack[len - 1] === ifEnum.CASECOND; len--)
                counter.nestingStack.pop();
        else
            counter.nestingStack.pop();
    }

    //Count operators
    for (let pos = 0; line.includes(';', pos); pos = line.indexOf(';', pos) + 1)
       counter.op++;
});

rl.on('close', function () {
    console.log('Operators:', counter.op, '\nConditional operators:', counter.condOp, '\nRatio:', counter.condOp / counter.op, '\nMaximum nesting:', counter.maxNesting)
});